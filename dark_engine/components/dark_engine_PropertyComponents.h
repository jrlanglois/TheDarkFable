//==============================================================================
/** */
template<typename StructureType, typename FlagEnum, int numFlags>
class FlagTickerPropertyComponent : public PropertyComponent,
                                    public LanguageHandler::Listener,
                                    private AsyncUpdater
{
public:
    FlagTickerPropertyComponent (const Value& v,
                                 const Identifier& id,
                                 int buttonHeight_ = 25) :
        PropertyComponent (id.toString(), buttonHeight_ * (numFlags + 1)),
        value (v),
        buttonHeight (buttonHeight_)
    {
        grid.autoRows = grid.autoColumns = Grid::TrackInfo (1_fr);
        grid.templateColumns.add (Grid::TrackInfo (1_fr));

        triggerAsyncUpdate();
    }

    /** @internal */
    void refresh() override                                 { triggerAsyncUpdate(); }
    /** @internal */
    void handleAsyncUpdate() override                       { populateButtons(); }
    /** @internal */
    void languageChanged (const IETFLanguageFile&) override { triggerAsyncUpdate(); }

    /** @internal */
    void resized() override
    {
        grid.performLayout (getLookAndFeel().getPropertyComponentContentPosition (*this));
    }

private:
    Value value;
    const int buttonHeight;
    OwnedArray<ToggleButton> buttons;
    Grid grid;

    StructureType getValueAsStructure() const
    {
        return VariantConverter<StructureType>::fromVar (value.getValue());
    }

    void populateButtons()
    {
        static const Identifier buttonIndexId = "buttonIndex";

        grid.items.clearQuick();
        grid.templateRows.clearQuick();
        buttons.clearQuick (true);

        for (int i = 0; i < numFlags; ++i)
        {
            auto flagName = darkEngine::toString (StructureType ((FlagEnum) 1 << i));
            auto* button = buttons.add (new ToggleButton (flagName));
            button->getProperties().set (buttonIndexId, i);

            button->onClick = [button, this]()
            {
                const auto index = static_cast<int> (button->getProperties()[buttonIndexId]);
                auto newVal = getValueAsStructure().toBitset();
                newVal.set ((size_t) index, button->getToggleState());
                value = VariantConverter<StructureType>::toVar (StructureType (newVal));
            };

            addAndMakeVisible (button);

            grid.items.add (button);
            grid.templateRows.add (Grid::TrackInfo (1_fr));
        }

        resized();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlagTickerPropertyComponent)
};

//==============================================================================
/** */
template<typename FlagType, int numFlags>
class EnumPickerPropertyComponent : public PropertyComponent,
                                    public LanguageHandler::Listener,
                                    private AsyncUpdater
{
public:
    EnumPickerPropertyComponent (const Value& v,
                                 const String& propertyName,
                                 int preferredHeight_ = 25) :
        PropertyComponent (propertyName, preferredHeight_),
        value (v)
    {
        addAndMakeVisible (comboBox);
        triggerAsyncUpdate();
    }

    FlagType fromIndex (int index) const                    { return static_cast<FlagType> (index); }
    void refresh() override                                 { triggerAsyncUpdate(); }
    void handleAsyncUpdate() override                       { populateComboBox(); }
    void languageChanged (const IETFLanguageFile&) override { triggerAsyncUpdate(); }

private:
    Value value;
    ComboBox comboBox;

    void populateComboBox()
    {
        comboBox.clear();

        for (int i = 0; i < numFlags; ++i)
            comboBox.addItem (darkEngine::toString (fromIndex (i)), i + 1);

        comboBox.setSelectedId (static_cast<int> (value.getValue()) + 1, dontSendNotification);
        repaint();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnumPickerPropertyComponent)
};

/** Material */
using MaterialEnumPickerPropertyComp = EnumPickerPropertyComponent<Material, static_cast<int> (Material::numMaterials)>;
/** MoveType */
using MoveTypeEnumPickerPropertyComponent = EnumPickerPropertyComponent<MoveType, static_cast<int> (MoveType::numMoveTypes)>;
/** StatusCondition */
using StatusConditionPickerPropertyComp = FlagTickerPropertyComponent<StatusCondition, StatusCondition::Flags, StatusCondition::numFlags>;
/** Difficulty */
using DifficultyPickerPropertyComp = FlagTickerPropertyComponent<Difficulty, Difficulty::Flags, Difficulty::numFlags>;

//==============================================================================
/** Material */
class MaterialPropertyParser final : public PropertyParser
{
public:
    /** */
    MaterialPropertyParser() = default;

    /** @internal */
    bool canUnderstand (const ValueTree&, const Identifier& id, const var& prop) const override
    {
        return prop.isInt() && id == materialId;
    }

    /** @internal */
    String toString (const Identifier&, const var& prop) const override
    {
        const auto mat = VariantConverter<Material>::fromVar (prop);
        return darkEngine::toString (mat, false);
    }

    /** @internal */
    std::unique_ptr<PropertyComponent> createPropertyComponent (const Value& valueToControl,
                                                                const Identifier&,
                                                                const String& propertyName) const override
    {
        return std::make_unique<MaterialPropertyComponent> (valueToControl, propertyName);
    }

private:
    /** Material */
    class MaterialPropertyComponent final : public MaterialEnumPickerPropertyComp
    {
    public:
        MaterialPropertyComponent (const Value& valueToControl,
                                   const String& propertyName) :
            MaterialEnumPickerPropertyComp (valueToControl, propertyName)
        {
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MaterialPropertyComponent)
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MaterialPropertyParser)
};

//==============================================================================
/** MoveType */
class MoveTypePropertyParser final : public PropertyParser
{
public:
    /** */
    MoveTypePropertyParser() = default;

    /** @internal */
    bool canUnderstand (const ValueTree&, const Identifier& id, const var& prop) const override
    {
        return prop.isInt() && (id == weakAgainstTypeId || id == moveTypeId);
    }

    /** @internal */
    String toString (const Identifier&, const var& prop) const override
    {
        const auto mt = VariantConverter<MoveType>::fromVar (prop);
        return darkEngine::toString (mt);
    }

    /** @internal */
    std::unique_ptr<PropertyComponent> createPropertyComponent (const Value& valueToControl,
                                                                const Identifier&,
                                                                const String& propertyName) const override
    {
        return std::make_unique<MoveTypePropertyComponent> (valueToControl, propertyName);
    }

private:
    /** MoveType */
    class MoveTypePropertyComponent final : public MoveTypeEnumPickerPropertyComponent
    {
    public:
        MoveTypePropertyComponent (const Value& valueToControl,
                                   const String& propertyName) :
            MoveTypeEnumPickerPropertyComponent (valueToControl, propertyName)
        {
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoveTypePropertyComponent)
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoveTypePropertyParser)
};

//==============================================================================
/** StatusCondition */
class StatusConditionPropertyParser final : public PropertyParser
{
public:
    /** */
    StatusConditionPropertyParser() = default;

    /** @internal */
    bool canUnderstand (const ValueTree&, const Identifier& id, const var& prop) const override
    {
        return prop.isInt() && id == statusConditionId;
    }

    /** @internal */
    String toString (const Identifier&, const var& prop) const override
    {
        return darkEngine::toString (VariantConverter<StatusCondition>::fromVar (prop));
    }

    /** @internal */
    std::unique_ptr<PropertyComponent> createPropertyComponent (const Value& valueToControl,
                                                                const Identifier&,
                                                                const String& propertyName) const override
    {
        return std::make_unique<StatusConditionPropertyComponent> (valueToControl, propertyName);
    }

private:
    /** StatusCondition */
    class StatusConditionPropertyComponent final : public StatusConditionPickerPropertyComp
    {
    public:
        StatusConditionPropertyComponent (const Value& valueToControl,
                                          const String& propertyName) :
            StatusConditionPickerPropertyComp (valueToControl, propertyName)
        {
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatusConditionPropertyComponent)
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatusConditionPropertyParser)
};

//==============================================================================
/** Difficulty */
class DifficultyPropertyParser final : public PropertyParser
{
public:
    /** */
    DifficultyPropertyParser() = default;

    /** @internal */
    bool canUnderstand (const ValueTree&, const Identifier& id, const var& prop) const override
    {
        return prop.isInt() && id == difficultyId;
    }

    /** @internal */
    String toString (const Identifier&, const var& prop) const override
    {
        const auto d = VariantConverter<Difficulty>::fromVar (prop);
        return darkEngine::toString (d);
    }

    /** @internal */
    std::unique_ptr<PropertyComponent> createPropertyComponent (const Value& valueToControl,
                                                                const Identifier&,
                                                                const String& propertyName) const override
    {
        return std::make_unique<DifficultyPropertyComponent> (valueToControl, propertyName);
    }

private:
    /** Difficulty */
    class DifficultyPropertyComponent final : public DifficultyPickerPropertyComp
    {
    public:
        DifficultyPropertyComponent (const Value& valueToControl,
                                     const String& propertyName) :
            DifficultyPickerPropertyComp (valueToControl, propertyName)
        {
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DifficultyPropertyComponent)
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DifficultyPropertyParser)
};

//==============================================================================
/** DoorLockState */
class DoorLockStatePropertyParser final : public PropertyParser
{
public:
    /** */
    DoorLockStatePropertyParser() = default;

    /** @internal */
    bool canUnderstand (const ValueTree&, const Identifier& id, const var& prop) const override
    {
        return prop.isInt() && id == lockStateId;
    }

    /** @internal */
    String toString (const Identifier&, const var& prop) const override
    {
        const auto dls = VariantConverter<DoorLockState>::fromVar (prop);
        return darkEngine::toString (dls);
    }

    /** @internal */
    std::unique_ptr<PropertyComponent> createPropertyComponent (const Value& valueToControl,
                                                                const Identifier&,
                                                                const String& propertyName) const override
    {
        return std::make_unique<DoorLockStatePropertyComponent> (valueToControl, propertyName);
    }

private:
    //==============================================================================
    /** DoorLockState */
    class DoorLockStatePropertyComponent : public ChoicePropertyComponent
    {
    public:
        DoorLockStatePropertyComponent (const Value& valueToControl,
                                        const String& propertyName) :
            ChoicePropertyComponent (valueToControl, propertyName, states, indices),
            value (valueToControl)
        {
            choices = states;
        }

        /** @internal */
        void setIndex (int newIndex) override   { value = newIndex; }
        /** @internal */
        int getIndex() const override           { return static_cast<int> (value.getValue()); }

    private:
        Value value;

        static inline const StringArray states =
        {
            darkEngine::toString (DoorLockState::unlocked),
            darkEngine::toString (DoorLockState::needsKey),
            darkEngine::toString (DoorLockState::needsSpell),
            darkEngine::toString (DoorLockState::impassable)
        };

        static inline const Array<var> indices =
        {
            static_cast<int> (DoorLockState::unlocked),
            static_cast<int> (DoorLockState::needsKey),
            static_cast<int> (DoorLockState::needsSpell),
            static_cast<int> (DoorLockState::impassable)
        };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoorLockStatePropertyComponent)
    };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoorLockStatePropertyParser)
};

//==============================================================================
/** WindowTileType */
class WindowTileTypePropertyParser final : public PropertyParser
{
public:
    /** */
    WindowTileTypePropertyParser() = default;

    /** @internal */
    bool canUnderstand (const ValueTree& tree, const Identifier& id, const var& prop) const override
    {
        return tree.hasType (tileId) && id == windowTileSubtypeId && prop.isInt();
    }

    /** @internal */
    String toString (const Identifier&, const var& prop) const override
    {
        const auto wtt = VariantConverter<WindowTileType>::fromVar (prop);
        return darkEngine::toString (wtt);
    }

    /** @internal */
    std::unique_ptr<PropertyComponent> createPropertyComponent (const Value& valueToControl,
                                                                const Identifier&,
                                                                const String& propertyName) const override
    {
        return std::make_unique<WindowTileTypePropertyComponent> (valueToControl, propertyName);
    }

private:
    //==============================================================================
    /** WindowTileType */
    class WindowTileTypePropertyComponent : public ChoicePropertyComponent
    {
    public:
        WindowTileTypePropertyComponent (const Value& valueToControl,
                                         const String& propertyName) :
            ChoicePropertyComponent (valueToControl, propertyName, states, indices),
            value (valueToControl)
        {
            choices = states;
        }

        /** @internal */
        void setIndex (int newIndex) override   { value = newIndex; }
        /** @internal */
        int getIndex() const override           { return static_cast<int> (value.getValue()); }

    private:
        Value value;

        static inline const StringArray states =
        {
            darkEngine::toString (WindowTileType::permanentlyOpen),
            darkEngine::toString (WindowTileType::permanentlyClosed),
            darkEngine::toString (WindowTileType::openable),
            darkEngine::toString (WindowTileType::openableWithUnlockableId)
        };

        static inline const Array<var> indices =
        {
            static_cast<int> (WindowTileType::permanentlyOpen),
            static_cast<int> (WindowTileType::permanentlyClosed),
            static_cast<int> (WindowTileType::openable),
            static_cast<int> (WindowTileType::openableWithUnlockableId)
        };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowTileTypePropertyComponent)
    };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowTileTypePropertyParser)
};

