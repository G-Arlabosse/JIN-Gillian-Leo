#include <gtest/gtest.h>
/*#include "myMain.h"
#include "pugixml.hpp"
#include "Circle.h"
#include <iostream>
using namespace std;
TEST(TestReadXML, TestCircle) {
  string c_dump_ref =
      R"(Circle "testCircle", x: 0, y: 1, r: 2, color: "Black"
)";

  string sc1 = R"(<?xml version = "1.0"?>
                            <Circle label="testCircle" x="0" y="1" r="2" color="Black" />)";
  pugi::xml_document doc1;
  if (pugi::xml_parse_result result1 = doc1.load_string(sc1.c_str());
      !result1) {
    std::cerr << result1.description();
    exit(1);
  }
  Circle c(doc1.child("Circle"), nullptr);
  cout << c.dump("");

  EXPECT_EQ(c.dump(""), c_dump_ref);
}

TEST(TestAnalyseXML, TestGroup) {
  string c_dump_ref =
      R"(Group "testGroup", x: 0, y: 1, children: [
| Circle "testCircle1", x: 2, y: 3, r: 4, color: "Black"
| Circle "testCircle2", x: 5, y: 6, r: 7, color: "Black"
]
)";

  string sc2 = R"(<?xml version = "1.0"?>
        <Group label="testGroup" x="0" y="1">
          <Circle label="testCircle1" x="2" y="3" r="4" color="Black"/>
          <Circle label="testCircle2" x="5" y="6" r="7" color="Black"/>
        </Group>)";
  pugi::xml_document doc2;
  if (pugi::xml_parse_result result2 = doc2.load_string(sc2.c_str());
      !result2) {
    std::cerr << result2.description();
    exit(1);
  }
  Group g(doc2.child("Group"), nullptr);

  EXPECT_EQ(g.dump(""), c_dump_ref);
}

TEST(TestReadXML, TestGroupHybrid) {
  string c_dump_ref =
      R"(Group "testGroupHybrid", x: 0, y: 1, children: [
| Circle "testCircle1", x: 2, y: 3, r: 4, color: "Black"
| Group "testGroup", x: 5, y: 6, children: [
| | Circle "testCircle2", x: 7, y: 8, r: 9, color: "Black"
| ]
| Circle "testCircle3", x: 10, y: 11, r: 12, color: "Black"
]
)";

  string sc2 = R"(<?xml version = "1.0"?>
                  <Group label="testGroupHybrid" x="0" y="1">
                    <Circle label="testCircle1" x="2" y="3" r="4" color="Black"/>
                    <Group label="testGroup" x="5" y="6">
                      <Circle label="testCircle2" x="7" y="8" r="9" color="Black"/>
                    </Group>
                    <Circle label="testCircle3" x="10" y="11" r="12" color="Black"/>
                  </Group>)";
  pugi::xml_document doc2;
  if (pugi::xml_parse_result result2 = doc2.load_string(sc2.c_str());
      !result2) {
    std::cerr << result2.description();
    exit(1);
  }
  Group g(doc2.child("Group"), nullptr);

  EXPECT_EQ(g.dump(""), c_dump_ref);
}*/

