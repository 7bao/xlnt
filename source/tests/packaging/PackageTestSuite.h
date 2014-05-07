#pragma once

#include <cxxtest/TestSuite.h>

#include <xlnt/file.h>
#include <xlnt/package.h>
#include "../../misc/pugixml.hpp"

class ZipPackageTestSuite : public CxxTest::TestSuite
{
public:
    ZipPackageTestSuite()
    {
        xlnt::file::copy("../../source/tests/packaging/test.zip", "../../source/tests/packaging/a.zip", true);
    }

    void test_read_text()
    {
        auto package = xlnt::package::open("../../source/tests/packaging/a.zip", xlnt::file_mode::Open, xlnt::file_access::ReadWrite);
        TS_ASSERT_DIFFERS(package, nullptr);

        auto part_1 = package.get_part(xlnt::uri("a.txt", xlnt::uri_kind::Relative));
        TS_ASSERT_DIFFERS(part_1, nullptr);

        auto part_1_data = part_1.read();
        TS_ASSERT_EQUALS(part_1_data, "a.txt");
    }

    void test_write_text()
    {
        {
            auto package = xlnt::package::open("../../source/tests/packaging/a.zip", xlnt::file_mode::Open, xlnt::file_access::ReadWrite);
            TS_ASSERT_DIFFERS(package, nullptr);

            auto part_1 = package.get_part(xlnt::uri("a.txt", xlnt::uri_kind::Relative));
            TS_ASSERT_DIFFERS(part_1, nullptr);

            part_1.write("something else");
        }

        {
            auto package = xlnt::package::open("../../source/tests/packaging/a.zip", xlnt::file_mode::Open, xlnt::file_access::ReadWrite);
            TS_ASSERT_DIFFERS(package, nullptr);

            auto part_1 = package.get_part(xlnt::uri("a.txt", xlnt::uri_kind::Relative));
            TS_ASSERT_DIFFERS(part_1, nullptr);

            auto part_1_data = part_1.read();

            TS_ASSERT_EQUALS(part_1_data, "something else");
        }
    }

    void test_read_xml()
    {
        auto package = xlnt::package::open("../../source/tests/packaging/a.zip", xlnt::file_mode::Open, xlnt::file_access::ReadWrite);
        TS_ASSERT_DIFFERS(package, nullptr);

        auto part_2 = package.get_part(xlnt::uri("a.xml", xlnt::uri_kind::Relative));
        TS_ASSERT_DIFFERS(part_2, nullptr);

        auto part_2_data = part_2.read();
        TS_ASSERT_DIFFERS(part_2_data, "");

        pugi::xml_document part_2_doc;
        part_2_doc.load(part_2_data.c_str());

        auto root_element = part_2_doc.child("root");
        TS_ASSERT_DIFFERS(root_element, nullptr)

        auto child_element = root_element.child("child");
        TS_ASSERT_DIFFERS(child_element, nullptr)

        TS_ASSERT_EQUALS(std::string(child_element.attribute("attribute").as_string()), "attribute")

        auto element_element = root_element.child("element");
        TS_ASSERT_DIFFERS(element_element, nullptr)

        TS_ASSERT_EQUALS(std::string(element_element.text().as_string()), "Text")
    }
};