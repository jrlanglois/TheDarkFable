#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace branding
{
    namespace logo
    {
        const auto background           = Colour (0xff2c2c2c);
        const auto pinkish              = Colour (0xffe7328b);
        const auto pinkishBackground    = Colour (0xff3f2d36);
        const auto greyish              = Colour (0xff919191);
    }

    namespace cmykLight
    {
        const auto darkest  = Colour (0xffccc2c0).darker();
        const auto cyan     = Colour (0xffadbecf); // Pastel Blue
        const auto magenta  = Colour (0xfff7c8c8); // Baby Pink
        const auto yellow   = Colour (0xfff7e8ca); // Champagne
        const auto black    = Colour (0xffccc2c0); // Pale Silver
    }
}

namespace dimensions
{
    enum
    {
        margin = 8,
        barDepth = 50,
        maxTooltipWidth = 256,

        indicatorAreaWidth = 10
    };

    const auto defaultTrackInfo     = Grid::TrackInfo (1_fr);
    constexpr auto cornerSize       = MathConstants<float>::pi;
    constexpr auto defaultFontSize  = 24.0f;
    constexpr auto tooltipFontSize  = defaultFontSize;

    inline void applyDefaults (Grid& grid)
    {
        grid.templateRows.add (defaultTrackInfo);

        grid.columnGap      = grid.rowGap   = Grid::Px (margin);
        grid.autoColumns    = grid.autoRows = defaultTrackInfo;
        grid.justifyItems   = Grid::JustifyItems::center;
        grid.alignItems     = Grid::AlignItems::center;
        grid.justifyContent = Grid::JustifyContent::spaceEvenly;
        grid.alignContent   = Grid::AlignContent::spaceEvenly;
        grid.autoFlow       = Grid::AutoFlow::columnDense;
    }
}

//==============================================================================
/** The global and all-encompassing set of shared fonts for our application. */
class SharedFonts final
{
public:
    //==============================================================================
    /** */
    SharedFonts();

    //==============================================================================
    /** Loads up the typeface and font instances.

        This will only load up things once by checking
        if the objects are valid or not.
    */
    void initialise();

    //==============================================================================
    std::unique_ptr<FontFamily> kodeMono, notoSans;
    FontFamily* defaultFamily = nullptr;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SharedFonts)
};

//==============================================================================
/** The global and all-encompassing set of shared resources for our application. */
class SharedResources final
{
public:
    /** */
    SharedResources() = default;

    //==============================================================================
    /** */
    void initialise();

    //==============================================================================
    SharedFonts sharedFonts;
    static inline const LookAndFeel_V4::ColourScheme colourScheme = LookAndFeel_V4::getDarkColourScheme();

private:
    //==============================================================================
    bool hasInitialised = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SharedResources)
};

//==============================================================================
/** */
class DarkFableLookAndFeel final : public LookAndFeel_V4
{
public:
    /** */
    DarkFableLookAndFeel();

    /** */
    ~DarkFableLookAndFeel() override;

    //==============================================================================
    /** */
    static void drawTextInRegion (Graphics& g, const Font& f, const String& text, Justification j,
                                  const Rectangle<int>& region, Colour colour = Colours::darkgrey, bool isEnabled = true);

    /** */
    static Grid createDefaultGrid();

    //==============================================================================
    /** @internal */
    int getMenuWindowFlags() override { return 0; }
    /** @internal */
    int getDefaultMenuBarHeight() override { return roundToInt (dimensions::defaultFontSize * 1.5f); }
    /** @internal */
    Font getMenuBarFont (MenuBarComponent&, int, const String&) override;
    /** @internal */
    void drawMenuBarBackground (Graphics&, int, int, bool, MenuBarComponent&) override;
    /** @internal */
    void drawMenuBarItem (Graphics&, int, int, int, const String&, bool, bool, bool, MenuBarComponent&) override;
    /** @internal */
    int getAlertBoxWindowFlags() override { return ComponentPeer::windowAppearsOnTaskbar; }
    /** @internal */
    void playAlertSound() override { }
    /** @internal */
    Typeface::Ptr getTypefaceForFont (const Font&) override;
    /** @internal */
    Font getComboBoxFont (ComboBox&) override;
    /** @internal */
    Rectangle<int> getTooltipBounds (const String&, Point<int>, Rectangle<int>) override;
    /** @internal */
    void drawTooltip (Graphics&, const String&, int, int) override;
    /** @internal */
    Font getPopupMenuFont() override;
    /** @internal */
    void drawLabel (Graphics&, Label&) override;
    /** @internal */
    Font getLabelFont (Label& label) override;
    /** @internal */
    BorderSize<int> getLabelBorderSize (Label&) override { return {}; }
    /** @internal */
    int getTabButtonSpaceAroundImage() override { return 0; }
    /** @internal */
    int getTabButtonOverlap (int) override { return 0; }
    /** @internal */
    int getTabButtonBestWidth (TabBarButton&, int) override;
    /** @internal */
    Rectangle<int> getTabButtonExtraComponentBounds (const TabBarButton&, Rectangle<int>&, Component&) override;
    /** @internal */
    void drawTabButton (TabBarButton&, Graphics&, bool, bool) override;
    /** @internal */
    Font getTabButtonFont (TabBarButton&, float) override;
    /** @internal */
    void drawTabButtonText (TabBarButton&, Graphics&, bool, bool) override;
    /** @internal */
    void drawTabbedButtonBarBackground (TabbedButtonBar&, Graphics&) override { }
    /** @internal */
    void drawTabAreaBehindFrontButton (TabbedButtonBar&, Graphics&, int, int) override { }
    /** @internal */
    void createTabButtonShape (TabBarButton&, Path&, bool, bool) override;
    /** @internal */
    void fillTabButtonShape (TabBarButton&, Graphics&, const Path&, bool, bool) override;
    /** @internal */
    Button* createTabBarExtrasButton() override;
    /** @internal */
    void drawComboBox (Graphics&, int, int, bool, int, int, int, int, ComboBox&) override;
    /** @internal */
    void drawButtonBackground (Graphics&, Button&, const Colour&, bool, bool) override;
    /** @internal */
    Font getTextButtonFont (TextButton&, int) override;
    /** @internal */
    int getTextButtonWidthToFitText (TextButton&, int) override;
    /** @internal */
    void drawButtonText (Graphics&, TextButton&, bool, bool) override;
    /** @internal */
    void drawToggleButton (Graphics&, ToggleButton&, bool, bool) override;
    /** @internal */
    void changeToggleButtonWidthToFitText (ToggleButton&) override;
    /** @internal */
    void drawTickBox (Graphics&, Component&, float, float, float, float, bool, bool, bool, bool) override;
    /** @internal */
    void drawDrawableButton (Graphics&, DrawableButton&, bool, bool) override;
    /** @internal */
    void fillTextEditorBackground (Graphics&, int, int, TextEditor&) override;
    /** @internal */
    void drawTextEditorOutline (Graphics&, int, int, TextEditor&) override;
    /** @internal */
    float getCallOutBoxCornerSize (const CallOutBox&) override { return dimensions::cornerSize; }
    /** @internal */
    void paintToolbarBackground (Graphics&, int, int, Toolbar&) override;
    /** @internal */
    int getDefaultScrollbarWidth() override { return 14; }
    /** @internal */
    bool areScrollbarButtonsVisible() override { return true; }
    /** @internal */
    void drawScrollbar (Graphics&, ScrollBar&, int, int, int, int, bool, int, int, bool, bool) override;
    /** @internal */
    void drawPopupMenuBackground (Graphics&, int, int) override;
    /** @internal */
    void drawStretchableLayoutResizerBar (Graphics&, int, int, bool, bool, bool) override;
    /** @internal */
    void drawTreeviewPlusMinusBox (Graphics&, const Rectangle<float>&, Colour, bool, bool) override;

    //==============================================================================
    SharedResourcePointer<SharedResources> sharedResources;

private:
    //==============================================================================
    TextLayout layoutTooltipText (const String& text, Colour textColour) const;
    Rectangle<float> getTooltipBoundsFloat (const String& tipText, Point<int> screenPos, Rectangle<int> parentArea) const;
    Font createUsefulFont (const Font& source) const;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DarkFableLookAndFeel)
};
