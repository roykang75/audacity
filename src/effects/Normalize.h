/**********************************************************************

  Audacity: A Digital Audio Editor

  Normalize.h

  Dominic Mazzoni
  Vaughan Johnson (Preview)

**********************************************************************/

#ifndef __AUDACITY_EFFECT_NORMALIZE__
#define __AUDACITY_EFFECT_NORMALIZE__

#include <wx/checkbox.h>
#include <wx/event.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>

#include "Effect.h"

class ShuttleGui;

#define NORMALIZE_PLUGIN_SYMBOL XO("Normalize")

class EffectNormalize final : public Effect
{
public:
   EffectNormalize();
   virtual ~EffectNormalize();

   // IdentInterface implementation

   wxString GetSymbol() override;
   wxString GetDescription() override;
   wxString ManualPage() override;

   // EffectDefinitionInterface implementation

   EffectType GetType() override;

   // EffectClientInterface implementation

   bool DefineParams( ShuttleParams & S ) override;
   bool GetAutomationParameters(CommandParameters & parms) override;
   bool SetAutomationParameters(CommandParameters & parms) override;

   // Effect implementation

   bool CheckWhetherSkipEffect() override;
   bool Startup() override;
   bool Process() override;
   void PopulateOrExchange(ShuttleGui & S) override;
   bool TransferDataToWindow() override;
   bool TransferDataFromWindow() override;

private:
   // EffectNormalize implementation

   bool ProcessOne(
      WaveTrack * t, const wxString &msg, int curTrackNum, float offset);
   bool AnalyseTrack(const WaveTrack * track, const wxString &msg,
                     int curTrackNum,
                     float &offset, float &min, float &max);
   void AnalyzeData(float *buffer, size_t len);
   bool AnalyseDC(const WaveTrack * track, const wxString &msg, int curTrackNum,
                  float &offset);
   void ProcessData(float *buffer, size_t len, float offset);

   void OnUpdateUI(wxCommandEvent & evt);
   void UpdateUI();

private:
   double mLevel;
   bool   mGain;
   bool   mDC;
   bool   mStereoInd;

   double mCurT0;
   double mCurT1;
   float  mMult;
   double mSum;
   sampleCount    mCount;

   wxCheckBox *mGainCheckBox;
   wxCheckBox *mDCCheckBox;
   wxTextCtrl *mLevelTextCtrl;
   wxStaticText *mLeveldB;
   wxStaticText *mWarning;
   wxCheckBox *mStereoIndCheckBox;

   bool mCreating;

   DECLARE_EVENT_TABLE()
};

#endif
