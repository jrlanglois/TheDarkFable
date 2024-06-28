#include "MainComponent.h"

//==============================================================================
class MainWindow final : public DocumentWindow
{
public:
    MainWindow (const String& name) :
        DocumentWindow (name, Colours::black, DocumentWindow::allButtons)
    {
        // getPeer()->setCurrentRenderingEngine (0);

        setUsingNativeTitleBar (true);
        Desktop::getInstance().setDefaultLookAndFeel (&lookAndFeel);
        setBackgroundColour (findColour (ResizableWindow::backgroundColourId));
        setLookAndFeel (&lookAndFeel);
        setContentOwned (new MainComponent(), true);

       #if SQUAREPINE_IS_MOBILE
        setFullScreen (true);
       #else
        setResizable (true, true);
        centreWithSize (getWidth(), getHeight());
       #endif

        setVisible (true);

#if 0
        const File f ("C:/Users/joelr/Music/alac/sample1.m4a");
        std::unique_ptr<AudioFormatReader> v (sp::ALACAudioFormat().createReaderFor (f.createInputStream().release(), true));
        if (v != nullptr)
        {
            DBG ("ALAC --- have reader");
        }
        else
        {
            DBG ("ALAC --- no reader");
        }
#endif

#if 0
        const File f ("C:/Users/joelr/Music/opus/sample1.opus");
        std::unique_ptr<AudioFormatReader> v (sp::OpusAudioFormat().createReaderFor (f.createInputStream().release(), true));
        if (v != nullptr)
        {
            DBG ("Opus --- have reader");
        }
        else
        {
            DBG ("Opus --- no reader");
        }
#endif
    }

    ~MainWindow() override
    {
        Desktop::getInstance().setDefaultLookAndFeel (nullptr);
        setLookAndFeel (nullptr);
    }

    //==============================================================================
    void closeButtonPressed() override
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }

private:
    //==============================================================================
    DarkFableLookAndFeel lookAndFeel;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

//==============================================================================
#if JUCE_DONT_DECLARE_PROJECTINFO
    #error "Need the project info..."
#endif

class TheDarkFableApplication final : public sp::SimpleApplication
{
public:
    TheDarkFableApplication() = default;

    //==============================================================================
    String getAppName() const override
    {
        String name;
        name.preallocateBytes (64);

        name
            << ProjectInfo::projectName
            << " by " << ProjectInfo::companyName
            << " (v" << getAppVersion() << ")";

       #if JUCE_DEBUG
        name << " --- [DEBUG]";
       #endif

        return name;
    }

    String getAppVersion() const override                           { return ProjectInfo::versionString; }
    std::unique_ptr<DocumentWindow> createWindow() const override   { return std::make_unique<MainWindow> (getAppName()); }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TheDarkFableApplication)
};

//==============================================================================
START_JUCE_APPLICATION (TheDarkFableApplication)
