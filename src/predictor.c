//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include "predictor.h"

//
// TODO:Student Information
//
//const char *studentName = "NAME";
//const char *studentID   = "PID";
//const char *email       = "EMAIL";
const char *studentName = "Tianyi Wang";
const char *studentID   = "A53274015";
const char *email       = "t9wang@ucsd.edu";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

//
//TODO: Add your own Branch Predictor data structures here
//
//static-none
//gshare:BHT, history
uint8_t *g_BHT;  // gshare BHT
uint32_t g_history;  // gshare global history
//tournament: local BHT, local PHT, global BHT, selector
uint8_t *localBHT;  // tournament local BHT 
uint32_t *localPHT;  // tournament local PHT 
uint8_t *globalBHT;  // tournament global BHT 
uint8_t *selector;  // selector fot tournament
uint8_t localOutcome;
uint8_t globalOutcome;



//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
void
init_predictor()
{
  //
  //TODO: Initialize Branch Predictor Data Structures
  //
  switch (bpType) {
    case STATIC:
      return;
    case GSHARE:
      g_history = 0;
      g_BHT = malloc((1<<ghistoryBits)*sizeof(uint8_t));
      memset(g_BHT, WN, sizeof(uint8_t)*(1<<ghistoryBits));
      break;
    case TOURNAMENT：
      break;
    case CUSTOM:
      break;
    default:
      break;
      
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
  //
  //TODO: Implement prediction scheme
  //

  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
      uint8_t result;
      int f = pc ^ g_history;
      int bits = ((1 << pcIndexBits) - 1);
      int index = f & bits;
      uint8_t predict = g_BHT[index];
      if (predict == WN){
        result = NOTTAKEN;
      }
      else{result = TAKEN;}
      
      return result;   
    case TOURNAMENT:
    case CUSTOM:
    default:
      break;
  }

  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void gshare_shift_predictor(uint32_t pc, uint8_t result) {
    int BHTindex = (pc ^ g_history) & ((1 << ghistoryBits) - 1);
    if (result == TAKEN) {
        if (g_BHT[BHTindex] != ST)
            g_BHT[BHTindex]++;
    } else {
        if (g_BHT[BHTindex] != SN)
            g_BHT[BHTindex]--;
    }
}  

void
train_predictor(uint32_t pc, uint8_t result)
{
  //
  //TODO: Implement Predictor training
  //
  switch (bpType) {
    case STATIC:
      return;
    case GSHARE:
      gshare_shift_predictor(pc, result);
      g_history <<= 1;
      g_history  &= ((1 << ghistoryBits) - 1);
      g_history |= result;
      break;
    case TOURNAMENT:
      tournament_update(pc, result);
      break;
    case CUSTOM:
      neural_train(pc, result);
      break;
    default:
      break;
    }
}
