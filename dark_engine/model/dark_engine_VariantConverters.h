namespace juce
{
    using namespace darkEngine;

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<StatusCondition> final
    {
        using Type = int;

        /** @returns */
        static StatusCondition fromVar (const var& v)   { return static_cast<Type> (v); }

        /** @returns */
        static var toVar (const StatusCondition& sc)    { return sc.getFlags(); }
    };

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<MoveType> final
    {
        using Type = int;

        /** @returns */
        static MoveType fromVar (const var& v)  { return static_cast<MoveType> (static_cast<Type> (v)); }

        /** @returns */
        static var toVar (MoveType mt)          { return static_cast<Type> (mt); }
    };

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<MoveCategory> final
    {
        using Type = int;

        /** @returns */
        static MoveCategory fromVar (const var& v)  { return static_cast<MoveCategory> (static_cast<Type> (v)); }

        /** @returns */
        static var toVar (MoveCategory mc)          { return static_cast<Type> (mc); }
    };

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<Nature> final
    {
        using Type = int;

        /** @returns */
        static Nature fromVar (const var& v)    { return static_cast<Nature> (static_cast<Type> (v)); }

        /** @returns */
        static var toVar (Nature n)             { return static_cast<Type> (n); }
    };

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<WeatherType> final
    {
        using Type = int;

        /** @returns */
        static WeatherType fromVar (const var& wt)  { return static_cast<WeatherType> (static_cast<Type> (wt)); }

        /** @returns */
        static var toVar (WeatherType wt)           { return static_cast<Type> (wt); }
    };

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<Difficulty> final
    {
        using Type = int;

        /** @returns */
        static Difficulty fromVar (const var& d)    { return static_cast<Type> (d); }

        /** @returns */
        static var toVar (Difficulty d)             { return static_cast<Type> (d.getFlags()); }
    };

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<Material> final
    {
        using Type = int;

        /** @returns */
        static Material fromVar (const var& v)  { return static_cast<Material> (static_cast<Type> (v)); }

        /** @returns */
        static var toVar (Material mat)         { return static_cast<Type> (mat); }
    };

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<DoorLockState> final
    {
        using Type = int;

        /** @returns */
        static DoorLockState fromVar (const var& v) { return static_cast<DoorLockState> (static_cast<Type> (v)); }

        /** @returns */
        static var toVar (DoorLockState dls)        { return static_cast<Type> (dls); }
    };

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<WindowTileType> final
    {
        using Type = int;

        /** @returns */
        static WindowTileType fromVar (const var& v)    { return static_cast<WindowTileType> (static_cast<Type> (v)); }

        /** @returns */
        static var toVar (WindowTileType wtt)           { return static_cast<Type> (wtt); }
    };
} // namespace juce
