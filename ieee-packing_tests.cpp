
#include <iostream>

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

#include "ieee-packing.hpp"

template<typename T> void test_float(const T value)
{
   T out;

   // const auto p1 = pack_float(value);
   // decltype(pack_float(value)) p2;
   // if constexpr(sizeof(T) == 4)
   //    p2 = detail::pack_float<float, 8>(value);
   // else
   //    p2 = detail::pack_float<double, 11>(value);

   // const auto pattern = (sizeof(T) == 4) ? "  0x%08x\n" : "  0x%016lx\n";

   // printf("------------------------ %a\n", value);
   // printf(pattern, p1);
   // printf(pattern, p2);
   // printf("\n");

   if constexpr(sizeof(T) == 4) {
      out = unpack_f32(pack_f32(value));
   } else {
      out = unpack_f64(pack_f64(value));
   }

   if(std::isfinite(value)) {
      const bool okay = fabs(value - out) <= std::numeric_limits<T>::epsilon();
      if(!okay) {
         std::cout << "testcase failed: sizeof(T) == " << sizeof(T) << "\n"
                   << "value = " << value << "\n"
                   << "out   = " << out << std::endl;
      }
      CATCH_REQUIRE(okay);
   } else {
      if(std::isnan(value)) {
         CATCH_REQUIRE(std::isnan(out));
      } else {
         CATCH_REQUIRE(std::isinf(out));
         CATCH_REQUIRE(std::signbit(value) == std::signbit(out));
      }
   }
}

template<typename T> void test_float_T()
{
   test_float<T>(std::numeric_limits<T>::quiet_NaN());
   test_float<T>(std::numeric_limits<T>::infinity());
   test_float<T>(-std::numeric_limits<T>::infinity());

   test_float<T>(std::numeric_limits<T>::max());
   test_float<T>(std::numeric_limits<T>::min());
   test_float<T>(std::numeric_limits<T>::lowest());
   test_float<T>(std::numeric_limits<T>::denorm_min());
   test_float<T>(std::numeric_limits<T>::epsilon());

   test_float<T>(T(M_PI));
   test_float<T>(T(M_PI) + std::numeric_limits<T>::epsilon());
   test_float<T>(T(M_PI) - std::numeric_limits<T>::epsilon());
   test_float<T>(T(0x81234567p1));
   test_float<T>(T(2.5));
   test_float<T>(T(0x1p1));
   test_float<T>(T(-1.0));
   test_float<T>(T(-0.0));
   test_float<T>(T(1.0));
   test_float<T>(T(1.0) + std::numeric_limits<T>::epsilon());
   test_float<T>(T(1.0) - std::numeric_limits<T>::epsilon());
   test_float<T>(0.5);
   test_float<T>(T(0.5) + std::numeric_limits<T>::epsilon());
   test_float<T>(T(0.5) - std::numeric_limits<T>::epsilon());
   test_float<T>(T(0.0));
   test_float<T>(T(0.0) + std::numeric_limits<T>::epsilon());
   test_float<T>(T(0.0) - std::numeric_limits<T>::epsilon());
}

CATCH_TEST_CASE("IEEE_PACKING_", "[ieee_packing]")
{
   //
   // -------------------------------------------------------
   //
   CATCH_SECTION("ieee_packing")
   {
      test_float_T<float>();
      test_float_T<double>();
   }
}
