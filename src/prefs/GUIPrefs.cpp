/**********************************************************************

  Audacity: A Digital Audio Editor

  GUIPrefs.cpp

  Brian Gunlogson
  Joshua Haberman
  Dominic Mazzoni
  James Crook


*******************************************************************//**

\class GUIPrefs
\brief A PrefsPanel for general GUI preferences.

*//*******************************************************************/

#include "../Audacity.h"
#include "GUIPrefs.h"

#include <wx/defs.h>

#include "../AudacityApp.h"
#include "../Languages.h"
#include "../Theme.h"
#include "../Prefs.h"
#include "../ShuttleGui.h"

#include "GUISettings.h"

#include "../Experimental.h"

#include "ThemePrefs.h"
#include "../AColor.h"
#include "../Internat.h"

GUIPrefs::GUIPrefs(wxWindow * parent, wxWindowID winid)
/* i18n-hint: refers to Audacity's user interface settings */
:  PrefsPanel(parent, winid, _("Interface"))
{
   Populate();
}

GUIPrefs::~GUIPrefs()
{
}

void GUIPrefs::GetRangeChoices(wxArrayString *pChoices, wxArrayString *pCodes)
{
   if (pCodes) {
      wxArrayString &codes = *pCodes;
      codes.Clear();
      codes.Add(wxT("36"));
      codes.Add(wxT("48"));
      codes.Add(wxT("60"));
      codes.Add(wxT("72"));
      codes.Add(wxT("84"));
      codes.Add(wxT("96"));
      codes.Add(wxT("120"));
      codes.Add(wxT("145"));
   }

   if (pChoices) {
      wxArrayString &choices = *pChoices;
      choices.Clear();
      choices.Add(_("-36 dB (shallow range for high-amplitude editing)"));
      choices.Add(_("-48 dB (PCM range of 8 bit samples)"));
      choices.Add(_("-60 dB (PCM range of 10 bit samples)"));
      choices.Add(_("-72 dB (PCM range of 12 bit samples)"));
      choices.Add(_("-84 dB (PCM range of 14 bit samples)"));
      choices.Add(_("-96 dB (PCM range of 16 bit samples)"));
      choices.Add(_("-120 dB (approximate limit of human hearing)"));
      choices.Add(_("-145 dB (PCM range of 24 bit samples)"));
   }
}

void GUIPrefs::Populate()
{
   // First any pre-processing for constructing the GUI.
   GetLanguages(mLangCodes, mLangNames);

   mHtmlHelpCodes.Add(wxT("Local"));
   mHtmlHelpCodes.Add(wxT("FromInternet"));

   mHtmlHelpChoices.Add(_("Local"));
   mHtmlHelpChoices.Add(_("From Internet"));

   mThemeCodes.Add( wxT("classic") );
   mThemeCodes.Add( wxT("light") );
   mThemeCodes.Add( wxT("dark") );
   mThemeCodes.Add( wxT("high-contrast") );
   mThemeCodes.Add( wxT("custom") );

   /* i18n-hint: describing the "classic" or traditional appearance of older versions of Audacity */
   mThemeChoices.Add( _("Classic") );
   /* i18n-hint: Light meaning opposite of dark */
   mThemeChoices.Add( _("Light") );
   mThemeChoices.Add( _("Dark") );
   /* i18n-hint: greater difference between foreground and background colors */
   mThemeChoices.Add( _("High Contrast") );
   /* i18n-hint: user defined */
   mThemeChoices.Add( _("Custom") );

   GetRangeChoices(&mRangeChoices, &mRangeCodes);

#if 0
   // only for testing...
   mLangCodes.Add("kg");   mLangNames.Add("Klingon");
   mLangCodes.Add("ep");   mLangNames.Add("Esperanto");
#endif

   //------------------------- Main section --------------------
   // Now construct the GUI itself.
   // Use 'eIsCreatingFromPrefs' so that the GUI is
   // initialised with values from gPrefs.
   ShuttleGui S(this, eIsCreatingFromPrefs);
   PopulateOrExchange(S);
   // ----------------------- End of main section --------------
}

void GUIPrefs::PopulateOrExchange(ShuttleGui & S)
{
   S.SetBorder(2);
   S.StartScroller();

   S.StartStatic(_("Display"));
   {
      S.StartMultiColumn(2);
      {

#ifdef EXPERIMENTAL_DA
         const wxString defaultTheme = wxT("dark");
#else
         const wxString defaultTheme = wxT("light");
#endif
         const wxString defaultRange = wxString::Format(wxT("%d"), ENV_DB_RANGE);

         S.TieChoice(_("&Language:"),
                     wxT("/Locale/Language"),
                     wxT(""),
                     mLangNames,
                     mLangCodes);
         S.SetSizeHints(mLangNames);

         S.TieChoice(_("Location of &Manual:"),
                     wxT("/GUI/Help"),
                     wxT("Local"),
                     mHtmlHelpChoices,
                     mHtmlHelpCodes);
         S.SetSizeHints(mHtmlHelpChoices);

         S.TieChoice(_("Th&eme:"),
                     wxT("/GUI/Theme"),
                     defaultTheme,
                     mThemeChoices,
                     mThemeCodes);
         S.SetSizeHints(mThemeChoices);

         S.TieChoice(_("Meter dB &range:"),
                     ENV_DB_KEY,
                     defaultRange,
                     mRangeChoices,
                     mRangeCodes);
         S.SetSizeHints(mRangeChoices);
      }
      S.EndMultiColumn();
//      S.AddSpace(10);
// JKC: This is a silly preference.  Kept here as a reminder that we may
// later want to have configurable button order.
//      S.TieCheckBox(_("&Ergonomic order of Transport Toolbar buttons"),
//                    wxT("/GUI/ErgonomicTransportButtons"),
//                    true);

   }
   S.EndStatic();

   S.StartStatic(_("Options"));
   {
      // Start wording of options with a verb, if possible.
      S.TieCheckBox(_("Show 'How to Get &Help' at launch"),
                    wxT("/GUI/ShowSplashScreen"),
                    true);
      S.TieCheckBox(_("Show e&xtra menus"),
                    wxT("/GUI/ShowExtraMenus"),
                    false);
#ifdef EXPERIMENTAL_THEME_PREFS
      // We do not want to make this option mainstream.  It's a 
      // convenience for developers.
      S.TieCheckBox(_("Show alternative &styling (Mac vs PC)"),
                    wxT("/GUI/ShowMac"),
                    false);
#endif
      S.TieCheckBox(_("&Beep on completion of longer activities"),
                    wxT("/GUI/BeepOnCompletion"),
                    false);
      S.TieCheckBox(_("Re&tain labels if selection snaps to a label"),
                    wxT("/GUI/RetainLabels"),
                    false);
      S.TieCheckBox(_("B&lend system and Audacity theme"),
                    wxT("/GUI/BlendThemes"),
                    true);
   }
   S.EndStatic();

   S.EndScroller();
}

bool GUIPrefs::Commit()
{
   ShuttleGui S(this, eIsSavingToPrefs);
   PopulateOrExchange(S);

   // If language has changed, we want to change it now, not on the next reboot.
   wxString lang = gPrefs->Read(wxT("/Locale/Language"), wxT(""));
   wxString usedLang = wxGetApp().InitLang(lang);
   // Bug 1523: Previously didn't check no-language (=System Language)
   if (!(lang.empty()) && (lang != usedLang)) {
      // lang was not usable and is not system language.  We got overridden.
      gPrefs->Write(wxT("/Locale/Language"), usedLang);
      gPrefs->Flush();
   }

   return true;
}

wxString GUIPrefs::HelpPageName()
{
   return "Interface_Preferences";
}

PrefsPanel *GUIPrefsFactory::operator () (wxWindow *parent, wxWindowID winid)
{
   wxASSERT(parent); // to justify safenew
   return safenew GUIPrefs(parent, winid);
}
