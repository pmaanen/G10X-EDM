// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME EdmGeantTEventDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
#include "EdmGeantTEvent.hh"
#include "EdmGeantParticleDB.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_EdmGeantTEvent(void *p = 0);
   static void *newArray_EdmGeantTEvent(Long_t size, void *p);
   static void delete_EdmGeantTEvent(void *p);
   static void deleteArray_EdmGeantTEvent(void *p);
   static void destruct_EdmGeantTEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EdmGeantTEvent*)
   {
      ::EdmGeantTEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EdmGeantTEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EdmGeantTEvent", ::EdmGeantTEvent::Class_Version(), "EdmGeantTEvent.hh", 178,
                  typeid(::EdmGeantTEvent), DefineBehavior(ptr, ptr),
                  &::EdmGeantTEvent::Dictionary, isa_proxy, 4,
                  sizeof(::EdmGeantTEvent) );
      instance.SetNew(&new_EdmGeantTEvent);
      instance.SetNewArray(&newArray_EdmGeantTEvent);
      instance.SetDelete(&delete_EdmGeantTEvent);
      instance.SetDeleteArray(&deleteArray_EdmGeantTEvent);
      instance.SetDestructor(&destruct_EdmGeantTEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EdmGeantTEvent*)
   {
      return GenerateInitInstanceLocal((::EdmGeantTEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EdmGeantTEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_EdmGeantTHit(void *p = 0);
   static void *newArray_EdmGeantTHit(Long_t size, void *p);
   static void delete_EdmGeantTHit(void *p);
   static void deleteArray_EdmGeantTHit(void *p);
   static void destruct_EdmGeantTHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EdmGeantTHit*)
   {
      ::EdmGeantTHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EdmGeantTHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EdmGeantTHit", ::EdmGeantTHit::Class_Version(), "EdmGeantTEvent.hh", 99,
                  typeid(::EdmGeantTHit), DefineBehavior(ptr, ptr),
                  &::EdmGeantTHit::Dictionary, isa_proxy, 4,
                  sizeof(::EdmGeantTHit) );
      instance.SetNew(&new_EdmGeantTHit);
      instance.SetNewArray(&newArray_EdmGeantTHit);
      instance.SetDelete(&delete_EdmGeantTHit);
      instance.SetDeleteArray(&deleteArray_EdmGeantTHit);
      instance.SetDestructor(&destruct_EdmGeantTHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EdmGeantTHit*)
   {
      return GenerateInitInstanceLocal((::EdmGeantTHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EdmGeantTHit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_EdmGeantTTrack(void *p = 0);
   static void *newArray_EdmGeantTTrack(Long_t size, void *p);
   static void delete_EdmGeantTTrack(void *p);
   static void deleteArray_EdmGeantTTrack(void *p);
   static void destruct_EdmGeantTTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EdmGeantTTrack*)
   {
      ::EdmGeantTTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EdmGeantTTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EdmGeantTTrack", ::EdmGeantTTrack::Class_Version(), "EdmGeantTEvent.hh", 139,
                  typeid(::EdmGeantTTrack), DefineBehavior(ptr, ptr),
                  &::EdmGeantTTrack::Dictionary, isa_proxy, 4,
                  sizeof(::EdmGeantTTrack) );
      instance.SetNew(&new_EdmGeantTTrack);
      instance.SetNewArray(&newArray_EdmGeantTTrack);
      instance.SetDelete(&delete_EdmGeantTTrack);
      instance.SetDeleteArray(&deleteArray_EdmGeantTTrack);
      instance.SetDestructor(&destruct_EdmGeantTTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EdmGeantTTrack*)
   {
      return GenerateInitInstanceLocal((::EdmGeantTTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EdmGeantTTrack*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_EdmGeantTRunProfile(void *p = 0);
   static void *newArray_EdmGeantTRunProfile(Long_t size, void *p);
   static void delete_EdmGeantTRunProfile(void *p);
   static void deleteArray_EdmGeantTRunProfile(void *p);
   static void destruct_EdmGeantTRunProfile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EdmGeantTRunProfile*)
   {
      ::EdmGeantTRunProfile *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EdmGeantTRunProfile >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EdmGeantTRunProfile", ::EdmGeantTRunProfile::Class_Version(), "EdmGeantTEvent.hh", 31,
                  typeid(::EdmGeantTRunProfile), DefineBehavior(ptr, ptr),
                  &::EdmGeantTRunProfile::Dictionary, isa_proxy, 4,
                  sizeof(::EdmGeantTRunProfile) );
      instance.SetNew(&new_EdmGeantTRunProfile);
      instance.SetNewArray(&newArray_EdmGeantTRunProfile);
      instance.SetDelete(&delete_EdmGeantTRunProfile);
      instance.SetDeleteArray(&deleteArray_EdmGeantTRunProfile);
      instance.SetDestructor(&destruct_EdmGeantTRunProfile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EdmGeantTRunProfile*)
   {
      return GenerateInitInstanceLocal((::EdmGeantTRunProfile*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EdmGeantTRunProfile*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
TClass *EdmGeantTEvent::fgIsA = 0;  // static to hold class pointer

//______________________________________________________________________________
const char *EdmGeantTEvent::Class_Name()
{
   return "EdmGeantTEvent";
}

//______________________________________________________________________________
const char *EdmGeantTEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EdmGeantTEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void EdmGeantTEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTEvent*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *EdmGeantTEvent::Class()
{
   if (!fgIsA) fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EdmGeantTHit::fgIsA = 0;  // static to hold class pointer

//______________________________________________________________________________
const char *EdmGeantTHit::Class_Name()
{
   return "EdmGeantTHit";
}

//______________________________________________________________________________
const char *EdmGeantTHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EdmGeantTHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void EdmGeantTHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTHit*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *EdmGeantTHit::Class()
{
   if (!fgIsA) fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EdmGeantTTrack::fgIsA = 0;  // static to hold class pointer

//______________________________________________________________________________
const char *EdmGeantTTrack::Class_Name()
{
   return "EdmGeantTTrack";
}

//______________________________________________________________________________
const char *EdmGeantTTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EdmGeantTTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void EdmGeantTTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTTrack*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *EdmGeantTTrack::Class()
{
   if (!fgIsA) fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EdmGeantTRunProfile::fgIsA = 0;  // static to hold class pointer

//______________________________________________________________________________
const char *EdmGeantTRunProfile::Class_Name()
{
   return "EdmGeantTRunProfile";
}

//______________________________________________________________________________
const char *EdmGeantTRunProfile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTRunProfile*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EdmGeantTRunProfile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTRunProfile*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void EdmGeantTRunProfile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTRunProfile*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *EdmGeantTRunProfile::Class()
{
   if (!fgIsA) fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EdmGeantTRunProfile*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
void EdmGeantTEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdmGeantTEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EdmGeantTEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(EdmGeantTEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EdmGeantTEvent(void *p) {
      return  p ? new(p) ::EdmGeantTEvent : new ::EdmGeantTEvent;
   }
   static void *newArray_EdmGeantTEvent(Long_t nElements, void *p) {
      return p ? new(p) ::EdmGeantTEvent[nElements] : new ::EdmGeantTEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_EdmGeantTEvent(void *p) {
      delete ((::EdmGeantTEvent*)p);
   }
   static void deleteArray_EdmGeantTEvent(void *p) {
      delete [] ((::EdmGeantTEvent*)p);
   }
   static void destruct_EdmGeantTEvent(void *p) {
      typedef ::EdmGeantTEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EdmGeantTEvent

//______________________________________________________________________________
void EdmGeantTHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdmGeantTHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EdmGeantTHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(EdmGeantTHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EdmGeantTHit(void *p) {
      return  p ? new(p) ::EdmGeantTHit : new ::EdmGeantTHit;
   }
   static void *newArray_EdmGeantTHit(Long_t nElements, void *p) {
      return p ? new(p) ::EdmGeantTHit[nElements] : new ::EdmGeantTHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_EdmGeantTHit(void *p) {
      delete ((::EdmGeantTHit*)p);
   }
   static void deleteArray_EdmGeantTHit(void *p) {
      delete [] ((::EdmGeantTHit*)p);
   }
   static void destruct_EdmGeantTHit(void *p) {
      typedef ::EdmGeantTHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EdmGeantTHit

//______________________________________________________________________________
void EdmGeantTTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdmGeantTTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EdmGeantTTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(EdmGeantTTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EdmGeantTTrack(void *p) {
      return  p ? new(p) ::EdmGeantTTrack : new ::EdmGeantTTrack;
   }
   static void *newArray_EdmGeantTTrack(Long_t nElements, void *p) {
      return p ? new(p) ::EdmGeantTTrack[nElements] : new ::EdmGeantTTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_EdmGeantTTrack(void *p) {
      delete ((::EdmGeantTTrack*)p);
   }
   static void deleteArray_EdmGeantTTrack(void *p) {
      delete [] ((::EdmGeantTTrack*)p);
   }
   static void destruct_EdmGeantTTrack(void *p) {
      typedef ::EdmGeantTTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EdmGeantTTrack

//______________________________________________________________________________
void EdmGeantTRunProfile::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdmGeantTRunProfile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EdmGeantTRunProfile::Class(),this);
   } else {
      R__b.WriteClassBuffer(EdmGeantTRunProfile::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EdmGeantTRunProfile(void *p) {
      return  p ? new(p) ::EdmGeantTRunProfile : new ::EdmGeantTRunProfile;
   }
   static void *newArray_EdmGeantTRunProfile(Long_t nElements, void *p) {
      return p ? new(p) ::EdmGeantTRunProfile[nElements] : new ::EdmGeantTRunProfile[nElements];
   }
   // Wrapper around operator delete
   static void delete_EdmGeantTRunProfile(void *p) {
      delete ((::EdmGeantTRunProfile*)p);
   }
   static void deleteArray_EdmGeantTRunProfile(void *p) {
      delete [] ((::EdmGeantTRunProfile*)p);
   }
   static void destruct_EdmGeantTRunProfile(void *p) {
      typedef ::EdmGeantTRunProfile current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EdmGeantTRunProfile

namespace {
  void TriggerDictionaryInitialization_EdmGeantTEventDict_Impl() {
    static const char* headers[] = {
"EdmGeantTEvent.hh",
"EdmGeantParticleDB.hh",
0
    };
    static const char* includePaths[] = {
"/usr/local/root/include",
"/Users/gogi/include",
"/usr/local/root/include",
"/Users/gogi/g4_devel/G10X-EDM/modules/EdmGeantTEvent/",
0
    };
    static const char* fwdDeclCode = 
"class EdmGeantTEvent;"
"class EdmGeantTHit;"
"class EdmGeantTTrack;"
"class EdmGeantTRunProfile;"
;
    static const char* payloadCode = 
"\n"
"#ifndef G__VECTOR_HAS_CLASS_ITERATOR\n"
"  #define G__VECTOR_HAS_CLASS_ITERATOR\n"
"#endif\n"
"\n"
"#define _BACKWARD_BACKWARD_WARNING_H\n"
"#include \"EdmGeantTEvent.hh\"\n"
"#include \"EdmGeantParticleDB.hh\"\n"
"\n"
"#undef  _BACKWARD_BACKWARD_WARNING_H\n"
;
    static const char* classesHeaders[]={
"EdmGeantTEvent", payloadCode, "@",
"EdmGeantTHit", payloadCode, "@",
"EdmGeantTRunProfile", payloadCode, "@",
"EdmGeantTTrack", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("EdmGeantTEventDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_EdmGeantTEventDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_EdmGeantTEventDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_EdmGeantTEventDict() {
  TriggerDictionaryInitialization_EdmGeantTEventDict_Impl();
}
