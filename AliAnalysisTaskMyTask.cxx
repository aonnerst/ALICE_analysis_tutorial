/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskMyTask
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */

#include "TChain.h"
#include "TH1F.h"
#include "TH3F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskMyTask.h"

class AliAnalysisTaskMyTask;    // your analysis class

using namespace std;            // std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskMyTask) // classimp: necessary for root

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask() : AliAnalysisTaskSE(), 
    fAOD(0), fOutputList(0), fHistPt(0), fHistPhi(0), fHistPhiCut810(0), fHistPhiCut610(0), fHistPhiCutLess8(0), fHistZetaPhi(0)
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char* name) : AliAnalysisTaskSE(name),
    fAOD(0), fOutputList(0), fHistPt(0), fHistPhi(0), fHistPhiCut810(0), fHistPhiCut610(0), fHistPhiCutLess8(0),fHistZetaPhi(0)
{
    // constructor
    DefineInput(0, TChain::Class());    // define the input of the analysis: in this case we take a 'chain' of events
                                        // this chain is created by the analysis manager, so no need to worry about it, 
                                        // it does its work automatically
    DefineOutput(1, TList::Class());    // define the ouptut of the analysis: in this case it's a list of histograms 
                                        // you can add more output objects by calling DefineOutput(2, classname::Class())
                                        // if you add more output objects, make sure to call PostData for all of them, and to
                                        // make changes to your AddTask macro!
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask()
{
    // destructor
    if(fOutputList) {
        delete fOutputList;     // at the end of your task, it is deleted from memory by calling this function
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserCreateOutputObjects()
{
    // create output objects
    //
    // this function is called ONCE at the start of your analysis (RUNTIME)
    // here you ceate the histograms that you want to use 
    //
    // the histograms are in this case added to a tlist, this list is in the end saved
    // to an output file
    //
    fOutputList = new TList();          // this is a list which will contain all of your histograms
                                        // at the end of the analysis, the contents of this list are written
                                        // to the output file
    fOutputList->SetOwner(kTRUE);       // memory stuff: the list is owner of all objects it contains and will delete them
                                        // if requested (dont worry about this now)

    // example of a histogram
    fHistPt = new TH1F("fHistPt", "fHistPt", 100, 0, 10);       // create your histogram
    fHistPhi = new TH1F("fHistPhi", "fHistPhi",50,-TMath::Pi(),TMath::Pi());
    fHistPhiCut810 = new TH1F("fHistPhiCut810", "fHistPhiCut810",50,-TMath::Pi(),TMath::Pi());
    fHistPhiCut610 = new TH1F("fHistPhiCut610", "fHistPhiCut610",50,-TMath::Pi(),TMath::Pi());
    fHistPhiCutLess8 = new TH1F("fHistPhiCutLess8", "fHistPhiCutLess8",50,-TMath::Pi(),TMath::Pi());
    fHistZetaPhi = new TH3F("fHistZetaPhi", "fHistZetaPhi", 50,-TMath::Pi(),TMath::Pi(), 200, -2.0, 2.0,
                        200, -10.0, 10.0);
    fOutputList->Add(fHistPt);          // don't forget to add it to the list! the list will be written to file, so if you want
    fOutputList->Add(fHistPhi);          // your histogram in the output file, add it to the list!
    fOutputList->Add(fHistPhiCut810);
    fOutputList->Add(fHistPhiCut610);
    fOutputList->Add(fHistPhiCutLess8);
    fOutputList->Add(fHistZetaPhi);
    
    PostData(1, fOutputList);           // postdata will notify the analysis manager of changes / updates to the 
                                        // fOutputList object. the manager will in the end take care of writing your output to file
                                        // so it needs to know what's in the output
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserExec(Option_t *)
{
    // user exec
    // this function is called once for each event
    // the manager will take care of reading the events from file, and with the static function InputEvent() you 
    // have access to the current event. 
    // once you return from the UserExec function, the manager will retrieve the next event from the chain
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());    // get an event (called fAOD) from the input file
                                                        // there's another event format (ESD) which works in a similar way
                                                        // but is more cpu/memory unfriendly. for now, we'll stick with aod's
    if(!fAOD) return;                                   // if the pointer to the event is empty (getting it failed) skip this event
        // example part: i'll show how to loop over the tracks in an event 
        // and extract some information from them which we'll store in a histogram
    AliVVertex *vtx = fAOD->GetPrimaryVertex();
    double zvtx = vtx->GetZ();
    
    Int_t iTracks(fAOD->GetNumberOfTracks());           // see how many tracks there are in the event
    for(Int_t i(0); i < iTracks; i++) {                 // loop over all these tracks
        AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));         // get a track (type AliAODTrack) from the event
        if(!track || !track->TestFilterBit(768)) continue;                            // if we failed, skip this track
        Char_t ch = (Char_t)track->Charge();
        if (ch==-1 || ch==1){
            double eta  = track -> Eta();
            double phi  = track -> Phi();
            double pt   = track -> Pt();
            fHistPt->Fill(pt);                     // plot the pt value of the track in a histogram
            fHistPhi->Fill(phi-TMath::Pi());
            fHistZetaPhi->Fill(phi-TMath::Pi(),eta,zvtx);
            if(zvtx>8.0 && zvtx<10.0 && eta>0.6 && eta<0.8){fHistPhiCut810->Fill(phi-TMath::Pi());}
            if(zvtx>6.0 && zvtx<10.0 && eta>0.6 && eta<0.8){fHistPhiCut610->Fill(phi-TMath::Pi());}
            if(zvtx<8.0 && eta>0.6 && eta<0.8){fHistPhiCutLess8->Fill(phi-TMath::Pi());}
        }
        
        
    }                                                   // continue until all the tracks are processed
    PostData(1, fOutputList);                           // stream the results the analysis of this event to
                                                         // the output manager which will take care of writing
                                                        // it to a file
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________
