// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIjbanedItritiumdITri_offlinedIPID_eff_scan_C_ACLiC_dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/home/jbane/tritium/Tri_offline/./PID_eff_scan.C"

// Header files passed via #pragma extra_include

namespace {
  void TriggerDictionaryInitialization_PID_eff_scan_C_ACLiC_dict_Impl() {
    static const char* headers[] = {
"./PID_eff_scan.C",
0
    };
    static const char* includePaths[] = {
"/u/apps/root/6.08.00/root/include",
"/u/apps/root/6.08.00/root/etc",
"/u/apps/root/6.08.00/root/etc/cling",
"/u/apps/root/6.08.00/root/include",
"/u/apps/gcc/5.2.0/install/bin/../lib/gcc/x86_64-unknown-linux-gnu/5.2.0/../../../../include/c++/5.2.0",
"/u/apps/gcc/5.2.0/install/bin/../lib/gcc/x86_64-unknown-linux-gnu/5.2.0/../../../../include/c++/5.2.0/x86_64-unknown-linux-gnu",
"/u/apps/gcc/5.2.0/install/bin/../lib/gcc/x86_64-unknown-linux-gnu/5.2.0/../../../../include/c++/5.2.0/backward",
"../src",
"/home/jbane/analyzer/analyzer/evio",
"/home/jbane/analyzer/analyzer/hana_decode/",
"/home/jbane/analyzer/analyzer/src",
"/home/jbane/analyzer/analyzer/hana_decode",
"/home/jbane/analyzer/analyzer/",
"/usr/include/freetype2",
"/u/apps/root/6.08.00/root/include",
"/home/jbane/tritium/Tri_offline/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "PID_eff_scan_C_ACLiC_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "PID_eff_scan_C_ACLiC_dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef __ACLIC__
  #define __ACLIC__ 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "./PID_eff_scan.C"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"PID_eff_scan", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("PID_eff_scan_C_ACLiC_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_PID_eff_scan_C_ACLiC_dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_PID_eff_scan_C_ACLiC_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_PID_eff_scan_C_ACLiC_dict() {
  TriggerDictionaryInitialization_PID_eff_scan_C_ACLiC_dict_Impl();
}
