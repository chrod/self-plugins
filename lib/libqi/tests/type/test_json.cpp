/*
** Copyright (C) 2010, 2012 Aldebaran Robotics
*/

#include <climits>
#include <float.h>
#include <cmath>
#include <gtest/gtest.h>
#include <map>
#include <qi/anyvalue.hpp>
#include <qi/application.hpp>
#include <qi/type/typeinterface.hpp>
#include <qi/jsoncodec.hpp>
#include <qi/log.hpp>

qiLogCategory("qi.test_json");

struct MPoint {
  MPoint(int x=0, int y=0)
    : x(x)
    , y(y)
  {}
  int x;
  int y;
};


QI_TYPE_STRUCT(MPoint, x, y);

TEST(TestJSON, MapIntTableString) {
  std::map<int, std::vector<std::string> > mps;

  std::vector<std::string> vs;
  vs.push_back("pif");
  vs.push_back("paf");
  vs.push_back("pof");

  mps[0] = vs;
  mps[2] = vs;

  qi::AnyReference gv = qi::AnyReference::from(mps);

  EXPECT_EQ("{ 0 : [ \"pif\", \"paf\", \"pof\" ], 2 : [ \"pif\", \"paf\", \"pof\" ] }", qi::encodeJSON(gv));
}

TEST(TestJSON, MST) {
  std::map<std::string, std::vector<std::string> > mps;

  std::vector<std::string> vs;
  vs.push_back("pif");
  vs.push_back("paf");
  vs.push_back("pof");

  mps["0"] = vs;
  mps["2"] = vs;

  qi::AnyReference gv = qi::AnyReference::from(mps);

  EXPECT_EQ("{ \"0\" : [ \"pif\", \"paf\", \"pof\" ], \"2\" : [ \"pif\", \"paf\", \"pof\" ] }", qi::encodeJSON(gv));

}

TEST(TestJSON, Simple) {
  EXPECT_EQ("true", qi::encodeJSON(bool(true)));
  EXPECT_EQ("false", qi::encodeJSON(bool(false)));
  EXPECT_EQ("32", qi::encodeJSON(32));
  EXPECT_EQ("\"ttc:42\"", qi::encodeJSON("ttc:42"));
  EXPECT_EQ("32.4", qi::encodeJSON(32.4f));
  EXPECT_EQ("32.3", qi::encodeJSON((double)32.3));

  qi::AnyValue gv(qi::TypeInterface::fromSignature(qi::Signature("c")));
  gv.setInt(42);
  EXPECT_EQ("42", qi::encodeJSON(gv));
}

TEST(TestJSON, SimpleAutoGV) {
  EXPECT_EQ("true", qi::encodeJSON(true));
  EXPECT_EQ("false", qi::encodeJSON(false));
  EXPECT_EQ("32", qi::encodeJSON(32));
  EXPECT_EQ("\"ttc:42\"", qi::encodeJSON("ttc:42"));
  EXPECT_EQ("32.4", qi::encodeJSON(32.4f));
  EXPECT_EQ("32.3", qi::encodeJSON((double)32.3));
}

TEST(TestJSON, String) {
  EXPECT_EQ("\" \\\" \"", qi::encodeJSON(" \" "));
  EXPECT_EQ("\" ' \"", qi::encodeJSON(" ' "));
  EXPECT_EQ("\" \\u0000 \"", qi::encodeJSON(" \0 "));
  EXPECT_EQ("\" \\u00E9 \"", qi::encodeJSON(" é "));

  EXPECT_EQ("\" \\\" \\u0000 \\u00E9 \"", qi::encodeJSON(" \" \0 é "));
}

TEST(TestJSON, CharTuple) {
  qi::AnyValue gv(qi::TypeInterface::fromSignature(qi::Signature("(c)")));
  EXPECT_EQ("[ 0 ]", qi::encodeJSON(gv));
}

TEST(TestJSON, EmptyValue) {
  qi::AnyValue gv(qi::TypeInterface::fromSignature(qi::Signature("m")));
  EXPECT_EQ("", qi::encodeJSON(gv));
}

TEST(TestJSON, Dynamics) {
  qi::AnyReference gv(qi::TypeInterface::fromSignature(qi::Signature("m")));
  qi::AnyValue gvr = qi::AnyValue::from("plouf");
  gv.setDynamic(gvr.asReference());
  EXPECT_EQ("\"plouf\"", qi::encodeJSON(gv));
}

TEST(TestJSON, NamedStruct) {
  MPoint mp(41, 42);
  qi::AnyValue gvr = qi::AnyValue::from(mp);
  EXPECT_EQ("{ \"x\" : 41, \"y\" : 42 }", qi::encodeJSON(gvr));
}

template<class T>
std::string itoa(T n)
{
  std::stringstream ss;

  ss << n;
  return ss.str();
}

bool pred(char const& c)
{
  bool res = ((c < '0' || c > '9')
              && c != '.'
      && c != '+' && c != '-'
      && c != 'e');
  return res;
}

std::string cleanStr(std::string const& numberStr)
{
  std::string copy = numberStr;
  size_t pos;

  if ((pos = copy.find("double")) != std::string::npos)
    copy.erase(pos, 6);
  std::string::iterator it = std::remove_if(copy.begin(), copy.end(), &pred);

  copy.erase(it, copy.end());
  return copy;
}

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

TEST(TestJSONDecoder, EmptyValue) {
  ASSERT_ANY_THROW(qi::decodeJSON(""));
}

TEST(TestJSONDecoder, String) {
  // Broken string
  EXPECT_ANY_THROW(qi::decodeJSON("\""));
  // Empty string
  EXPECT_NO_THROW(qi::decodeJSON("\"\""));
  // Normal string
  EXPECT_NO_THROW(qi::decodeJSON("\"test\""));

  // Test result
  EXPECT_STREQ("test", qi::decodeJSON("\"test\"").asString().c_str());

  // Test result with spaces before
  EXPECT_STREQ("   test", qi::decodeJSON("\"   test\"").asString().c_str());

  // Escape chars
  std::string testString =
      "\""
      "\\\""
      "\\\\"
      "\\/"
      "\\b"
      "\\f"
      "\\n"
      "\\r"
      "\\t"
      "\"";
  std::string resString =
      "\""
      "\\"
      "/"
      "\b"
      "\f"
      "\n"
      "\r"
      "\t";
  ASSERT_NO_THROW(qi::decodeJSON(testString));
  ASSERT_STREQ(resString.c_str(), qi::decodeJSON(testString).asString().c_str());
}

TEST(TestJSONDecoder, Integer) {
  // broken value
  EXPECT_ANY_THROW(qi::decodeJSON("--42"));

  ASSERT_NO_THROW(qi::decodeJSON("42"));
  ASSERT_NO_THROW(qi::decodeJSON("-42"));

  ASSERT_NO_THROW(qi::decodeJSON("42").asInt64());

  // positive value
  EXPECT_EQ(42, qi::decodeJSON("42").asInt64());

  // negative value
  EXPECT_EQ(-42, qi::decodeJSON("-42").asInt64());

  // max/min vals
  EXPECT_EQ(LONG_MAX, qi::decodeJSON(itoa(LONG_MAX)).asInt64());
  EXPECT_EQ(LONG_MIN, qi::decodeJSON(itoa(LONG_MIN)).asInt64());
}

TEST(TestJSONDecoder, Float) {
  // broken value
  ASSERT_NO_THROW(qi::decodeJSON("42.43"));
  ASSERT_NO_THROW(qi::decodeJSON("-42.43"));
  ASSERT_NO_THROW(qi::decodeJSON("0.42"));
  ASSERT_NO_THROW(qi::decodeJSON("42.0"));
  ASSERT_NO_THROW(qi::decodeJSON("0.0000042"));
  ASSERT_NO_THROW(qi::decodeJSON("0.42").asDouble());
  ASSERT_NO_THROW(qi::decodeJSON("40.40e+10"));
  ASSERT_NO_THROW(qi::decodeJSON("42e+10"));
  ASSERT_NO_THROW(qi::decodeJSON("42e-10"));
  ASSERT_NO_THROW(qi::decodeJSON("42e10"));

  EXPECT_DOUBLE_EQ(0.42, qi::decodeJSON("0.42").asDouble());
  EXPECT_DOUBLE_EQ(42.0, qi::decodeJSON("42.0").asDouble());
  EXPECT_DOUBLE_EQ(0.0000042, qi::decodeJSON("0.0000042").asDouble());

  // positive value
  EXPECT_DOUBLE_EQ(42.43, qi::decodeJSON("42.43").asDouble());

  // negative value
  EXPECT_DOUBLE_EQ(-42.43, qi::decodeJSON("-42.43").asDouble());

  // with E
  EXPECT_DOUBLE_EQ(42e+10, qi::decodeJSON("42e+10").asDouble());
  EXPECT_DOUBLE_EQ(42e-10, qi::decodeJSON("42e-10").asDouble());
  EXPECT_DOUBLE_EQ(42e10, qi::decodeJSON("42e10").asDouble());

  // max/min vals
  EXPECT_DOUBLE_EQ(DBL_MAX, qi::decodeJSON(cleanStr(STR(DBL_MAX))).asDouble());
  EXPECT_DOUBLE_EQ(DBL_MIN, qi::decodeJSON(cleanStr(STR(DBL_MIN))).asDouble());
}

TEST(TestJSONDecoder, Array) {
  // good parse and type
  ASSERT_NO_THROW(qi::decodeJSON("[]"));
  ASSERT_NO_THROW(qi::decodeJSON("[42]"));
  ASSERT_NO_THROW(qi::decodeJSON("[1, 2]"));
  ASSERT_NO_THROW(qi::decodeJSON("[1, 2, [1, 2, 3, [42], 45], 1.2]"));
  ASSERT_EQ(qi::TypeKind_List, qi::decodeJSON("[42]").kind());

  // size 1
  ASSERT_NO_THROW(qi::decodeJSON("[42]").size());
  ASSERT_EQ(1U, qi::decodeJSON("[42]").size());

  // size > 1
  ASSERT_EQ(6U, qi::decodeJSON("[1, 2, 3, 4, 5, 6]").size());

  // complex array
  qi::AnyValue val = qi::decodeJSON("[1, [2, 3]]");
  ASSERT_EQ(qi::TypeKind_List, val.kind());
  ASSERT_EQ(qi::TypeKind_Int, val[0].content().kind());
  ASSERT_EQ(qi::TypeKind_List, val[1].content().kind());
  ASSERT_EQ(2U, qi::decodeJSON("[1, [2, 3]]")[1].content().size());
}

TEST(TestJSONDecoder, Object) {
  // good parse
  ASSERT_NO_THROW(qi::decodeJSON("{}"));
  ASSERT_NO_THROW(qi::decodeJSON("{\"a\":42}"));
  ASSERT_NO_THROW(qi::decodeJSON("{\"a\":42, \"b\":1.0}"));
  ASSERT_NO_THROW(qi::decodeJSON("{\"a\":42, \"b\":{\"c\":[1, 2]}}"));

  ASSERT_ANY_THROW(qi::decodeJSON("{"));
  ASSERT_ANY_THROW(qi::decodeJSON("{42:42}"));
  ASSERT_ANY_THROW(qi::decodeJSON("{\"42\":}"));

  ASSERT_EQ(qi::TypeKind_Map, qi::decodeJSON("{}").kind());
  ASSERT_EQ(1U, qi::decodeJSON("{\"a\":42}").size());
  ASSERT_EQ(qi::TypeKind_Int, qi::decodeJSON("{\"a\":42}")["a"].content().kind());

}

TEST(TestJSONDecoder, special) {
  // good parse
  ASSERT_NO_THROW(qi::decodeJSON("true"));
  ASSERT_NO_THROW(qi::decodeJSON("false"));
  ASSERT_NO_THROW(qi::decodeJSON("null"));
  ASSERT_ANY_THROW(qi::decodeJSON("tru"));

  ASSERT_EQ(qi::TypeKind_Int, qi::decodeJSON("true").kind());
  ASSERT_EQ(qi::TypeKind_Int, qi::decodeJSON("false").kind());
  ASSERT_EQ(qi::TypeKind_Void, qi::decodeJSON("null").kind());

  ASSERT_EQ(0U, static_cast<qi::IntTypeInterface*>(qi::decodeJSON("true").type())->size());
  ASSERT_EQ(0U, static_cast<qi::IntTypeInterface*>(qi::decodeJSON("false").type())->size());

  ASSERT_EQ(1, qi::decodeJSON("true").toInt());
  ASSERT_EQ(0, qi::decodeJSON("false").toInt());
}

TEST(TestJSONDecoder, itOverload) {
  std::string testString = "<jsonString=\"[\"a\", 42]\"/>";

  qi::AnyValue val;
  ASSERT_NO_THROW(qi::decodeJSON(testString.begin() + 13, testString.end(), val));
  ASSERT_EQ('\"', *qi::decodeJSON(testString.begin() + 13, testString.end(), val));
  qi::decodeJSON(testString.begin() + 13, testString.end(), val);
  ASSERT_EQ(qi::TypeKind_List, val.kind());

  std::string testString2 = "<jsonString=\"[\"a\", 42\"/>";
  ASSERT_ANY_THROW(qi::decodeJSON(testString2.begin() + 13, testString2.end(), val));

}

TEST(TestJSONDecoder, Strings)
{
  static const std::string s1 = "{ \"content\" : \"pone\" }";
  static const std::string s2 = "{ \"content\" : \"poné\" }";
  static const std::string s3 = "{ \"content\" : \"pon\\u00e9\" }";
  static const std::string s4 = "{ \"content\" : \"pon\\u00ex\" }";
  static const std::string s5 = "{ \"content\" : \"pon\\u\"}";

  qi::AnyValue v;
  v = qi::decodeJSON(s1);
  EXPECT_EQ("pone", v["content"].to<std::string>());

  v = qi::decodeJSON(s2);
  EXPECT_EQ("poné", v["content"].to<std::string>());

  ASSERT_NO_THROW(v = qi::decodeJSON(s3));
  EXPECT_EQ("pon\xc3\xa9", v["content"].to<std::string>());

  ASSERT_ANY_THROW(qi::decodeJSON(s4));
  ASSERT_ANY_THROW(qi::decodeJSON(s5));
}

TEST(TestJSONDecoder, ignoringWhiteSpace)
{
  static const std::string ohMinceAlors =
      "{\n"
      "  \"petit\" : \"poney\"\n"
      "}\n";
  std::map<std::string, std::string> ahBahZut = qi::decodeJSON(ohMinceAlors)
      .toMap<std::string, std::string>();
  ASSERT_EQ(std::string("poney"), ahBahZut["petit"]);
}

struct Qiqi
{
  float ffloat;
  double fdouble;
  int fint;
  bool operator==(const Qiqi &other) const
  {
      return (ffloat == other.ffloat) &&
             (fdouble == other.fdouble) &&
             (fint == other.fint);
  }
};
QI_TYPE_STRUCT_REGISTER(Qiqi, ffloat, fdouble, fint);

TEST(TestJSONDecoder, StructWithDifferentSizedFields)
{
  Qiqi val;
  val.ffloat = 3.14f;
  val.fdouble = 5.5555;
  val.fint = 44;

  std::cout << qi::AnyValue(val).signature().toString() << std::endl;

  std::string json = qi::encodeJSON(val);
  qiLogDebug() << json << std::endl;
  qi::AnyValue res_any = qi::decodeJSON(json);
  qiLogDebug() << res_any.signature().toString() << std::endl;
  Qiqi res = res_any.to<Qiqi>();
  EXPECT_EQ(val,
            res) << qi::encodeJSON(val) << "\n" << qi::encodeJSON(res);
}

int main(int argc, char **argv)
{
  qi::Application app(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
