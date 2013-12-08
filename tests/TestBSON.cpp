/// \author James Hughes
/// \date   November 2013

#include <gtest/gtest.h>
#include <bson.h>

// Only tests compilation. No real unit tests here. That is covered by the
// external library's battery of tests.
TEST(Mongo, BSON)
{
  // Start forming a bson structure to send to the viewer.
  bson b;
  bson_init(&b);

  bson_append_int(&b, "frameTimeSecs", 1234);
  bson_append_int(&b, "frameTimeMS", 1234567);
  bson_append_double(&b, "experimentTime", 1.023);

  bson_finish(&b);
}


