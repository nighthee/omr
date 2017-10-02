/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/
 
#include <iostream>
#include <cstdint>
#include <cassert>

#include "Jit.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/ThunkBuilder.hpp"

static uintptr_t floatHelper(float f){
	union F
	{
		uintptr_t uip;
		float data;
	};
	F fUnion;
	fUnion.uip = 0;
	fUnion.data = f;
	return fUnion.uip;
}

static void
thunk_1b(int8_t b)
   {
   std::cout << "\tthunk_1b: b is " << b << "\n";
   return;
   }

static void
thunk_1b_2(int8_t b)
   {
   std::cout << "\tthunk_1b_2: b-1 is " << (char)(b-1) << "\n";
   return;
   }

static char*
thunk_1b_3(int8_t b)
   {
	char result = (char)b;
	char* ptr = &result;
   std::cout << "\tthunk_1b_3: b is " << result << "\n";
	return ptr;
   }

static char*
thunk_1b_4(int8_t b)
   {
	char result = (char)(b-1);
	char* ptr = &result;
   std::cout << "\tthunk_1b_4: b-1 is " << result << "\n";
   return ptr;
   }

static void
thunk_1s(int16_t s)
   {
   std::cout << "\tthunk_1s: s is " << s << "\n";
   return;
   }

static void
thunk_1s_2(int16_t s)
   {
   std::cout << "\tthunk_1s_2: s-1 is " << (s-1) << "\n";
   return;
   }

static void
thunk_1i(int32_t i)
   {
   std::cout << "\tthunk_1i: i is " << i << "\n";
   return;
   }

static void
thunk_1i_2(int32_t i)
   {
   std::cout << "\tthunk_1i_2: i-1 is " << (i-1) << "\n";
   return;
   }

static void
thunk_1l(int64_t l)
   {
   std::cout << "\tthunk_1l: l is " << l << "\n";
   return;
   }

static void
thunk_1l_2(int64_t l)
   {
   std::cout << "\tthunk_1l_2: l-1 is " << (l-1) << "\n";
   return;
   }

static void
thunk_1bool(uint8_t boolean)
   {
   std::cout << "\tthunk_1bool: boolean (1) is " << (bool)(boolean) << "\n";
   return;
   }
static void
thunk_1bool_2(uint8_t boolean)
   {
   std::cout << "\tthunk_1bool_2: boolean-1 (0) is " << (bool)(boolean-1) << "\n";
   return;
   }

static void
thunk_1c(uint16_t c)
   {
   std::cout << "\tthunk_1c: c (67) is " << (char)c << "\n";
   return;
   }

static void
thunk_1c_2(uint16_t c)
   {
   std::cout << "\tthunk_1c_2: c-1 (66) is " << (char)(c-1) << "\n";
   return;
   }

static void
thunk_1f(float f)
   {
	printf("Function: %G\n", f);
   return;
   }

static void
thunk_1f_2(float f)
   {
	float test = 4.5;
   std::cout << "\tthunk_1f_2: f-1 is " << test << "\n";
   return;
   }


typedef void (VoidThunkType)(void *, uintptr_t *);
typedef void (VoidThunkTypeFloat)(void *, float *);

int main(int argc, char *argv[])
   {
   std::cout << "Step 1: initialize JIT\n";
   bool jit_initialized = initializeJit();
   assert(jit_initialized);

   std::cout << "Step 2: create TR::TypeDictionary instance\n";
   TR::TypeDictionary d;

   uint32_t rc;
   uint8_t *startPC;

   std::cout << "Step 3: test primitive thunk returning void\n";

   TR::IlType *NoType = d.PrimitiveType(TR::NoType);

   TR::IlType *parmTypes_1b[] = { d.toIlType<int8_t>() };
   TR::ThunkBuilder thunk1b(&d, "1b", NoType, 1, parmTypes_1b);
   rc = compileMethodBuilder(&thunk1b, &startPC);
   if (rc == 0)
      {
      VoidThunkType *f = (VoidThunkType *)startPC;
      uintptr_t args[] = { (uintptr_t) 'x' };
      f((void*)&thunk_1b, args);
      f((void*)&thunk_1b_2, args);
      char result = f((char)&thunk_1b_3, args);
      std::cout << result << std::endl;
      char result = f((char)&thunk_1b_4, args);
      std::cout << result << std::endl;
      }

   TR::IlType *parmTypes_1s[] = { d.toIlType<int16_t>() };
   TR::ThunkBuilder thunk1s(&d, "1s", NoType, 1, parmTypes_1s);
   rc = compileMethodBuilder(&thunk1s, &startPC);
   if (rc == 0)
      {
      VoidThunkType *f = (VoidThunkType *)startPC;
      uintptr_t args[] = { (uintptr_t) 4097 };
      f((void*)&thunk_1s, args);
      f((void*)&thunk_1s_2, args);
      }

   TR::IlType *parmTypes_1i[] = { d.toIlType<int32_t>() };
   TR::ThunkBuilder thunk1i(&d, "1i", NoType, 1, parmTypes_1i);
   rc = compileMethodBuilder(&thunk1i, &startPC);
   if (rc == 0)
      {
      VoidThunkType *f = (VoidThunkType *)startPC;
      uintptr_t args[] = { (uintptr_t) 1233475 };
      f((void*)&thunk_1i, args);
      f((void*)&thunk_1i_2, args);
      }

   TR::IlType *parmTypes_1l[] = { d.toIlType<int64_t>() };
   TR::ThunkBuilder thunk1l(&d, "1l", NoType, 1, parmTypes_1l);
   rc = compileMethodBuilder(&thunk1l, &startPC);
   if (rc == 0)
      {
      VoidThunkType *f = (VoidThunkType *)startPC;
      uintptr_t args[] = { (uintptr_t) 1233475 };
      f((void*)&thunk_1l, args);
      f((void*)&thunk_1l_2, args);
      }

  TR::IlType *parmTypes_1c[] = { d.toIlType<uint16_t>() };
  TR::ThunkBuilder thunk1c(&d, "1c", NoType, 1, parmTypes_1c);
  rc = compileMethodBuilder(&thunk1c, &startPC);
  if (rc == 0)
	 {
	 VoidThunkType *f = (VoidThunkType *)startPC;
	 uintptr_t args[] = { (uintptr_t) 67 };
	 f((void*)&thunk_1c, args);
	 f((void*)&thunk_1c_2, args);
	 }

  TR::IlType *parmTypes_1bool[] = { d.toIlType<uint8_t>() };
  TR::ThunkBuilder thunk1bool(&d, "1bool", NoType, 1, parmTypes_1bool);
  rc = compileMethodBuilder(&thunk1bool, &startPC);
  if (rc == 0)
	{
	VoidThunkType *f = (VoidThunkType *)startPC;
	uintptr_t args[] = { (uintptr_t) 1 };
	f((void*)&thunk_1bool, args);
	f((void*)&thunk_1bool_2, args);
	}

  TR::IlType *parmTypes_1f[] = { d.toIlType<float>() };
  TR::ThunkBuilder thunk1f(&d, "1f", NoType, 1, parmTypes_1f);
  rc = compileMethodBuilder(&thunk1f, &startPC);
  if (rc == 0)
  	{
  	VoidThunkTypeFloat *f = (VoidThunkTypeFloat *)startPC;
  	float floatingPoint = 54.5f;
  	uintptr_t uip2 = 0;
  	/*uip2 = *(uintptr_t*)(uintptr_t)(float*)&floatingPoint;
  	std::cout<< "cout: " << uip2 << std::endl;
  	printf("Original: %G\n", floatingPoint);
  	printf("UIP: %G\n", uip2);*/
  	float args[] = { floatingPoint };
  	f((void*)&thunk_1f, (float*) args);
  	//f((void*)&thunk_1f_2, (uintptr_t*) args);
  	}

   std::cout << "Step 4: shutdown JIT\n";
   shutdownJit();
   
   std::cout << "PASS\n";
   }
