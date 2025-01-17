/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2023, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsitec.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

#include "common/stringutils.h"

#include <gtest/gtest.h>

namespace StringUtilsTesta
{

TEST(StringUtilTests, ReplaceShortToLong)
{
    std::string text = "Test {123}, {123}, {123}{123} Test";
    std::string expected = "Test [0987654], [0987654], [0987654][0987654] Test";

    auto result = StrUtils::Replace(text, "{123}", "[0987654]");

    EXPECT_EQ(result, expected);
}

TEST(StringUtilTests, ReplaceLongToShort)
{
    std::string text = "Test {1234567}, {1234567}, {1234567}{1234567} Test";
    std::string expected = "Test [64], [64], [64][64] Test";

    auto result = StrUtils::Replace(text, "{1234567}", "[64]");

    EXPECT_EQ(result, expected);
}

TEST(StringUtilTests, ReplaceSameLength)
{
    std::string text = "Test {123}, {123}, {123}{123} Test";
    std::string expected = "Test [432], [432], [432][432] Test";

    auto result = StrUtils::Replace(text, "{123}", "[432]");

    EXPECT_EQ(result, expected);
}

TEST(StringUtilTests, RemoveComments)
{
    std::string text = "qwerty";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, "qwerty");

    text = R"(qwerty // comment)";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, R"(qwerty )");

    text = R"(qwerty 'test' // comment)";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, R"(qwerty 'test' )");

    text = R"(qwerty "test" // comment)";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, R"(qwerty "test" )");

    text = R"(qwerty 'test // test')";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, R"(qwerty 'test // test')");

    text = R"(qwerty "test // test")";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, R"(qwerty "test // test")");

    text = R"(// comment "text")";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, "");

    text = R"(// comment 'text')";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, "");

    text = R"("qwerty"//comment)";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, R"("qwerty")");

    text = R"('qwerty'//comment)";
    StrUtils::RemoveComments(text);
    EXPECT_EQ(text, R"('qwerty')");
}

TEST(StringUtilTests, SplitSingle)
{
    std::string text = "CreateObject test value  123";

    auto result = StrUtils::Split(text, " ");

    ASSERT_EQ(result.size(), 4);

    EXPECT_EQ(result[0], "CreateObject");
    EXPECT_EQ(result[1], "test");
    EXPECT_EQ(result[2], "value");
    EXPECT_EQ(result[3], "123");
}

TEST(StringUtilTests, SplitMultiple)
{
    std::string text = "id=123, value=123,test=#45 no=yes";

    auto result = StrUtils::Split(text, " ,");

    ASSERT_EQ(result.size(), 4);

    EXPECT_EQ(result[0], "id=123");
    EXPECT_EQ(result[1], "value=123");
    EXPECT_EQ(result[2], "test=#45");
    EXPECT_EQ(result[3], "no=yes");
}

TEST(StringUtilTests, StringCodePointCounts)
{
    EXPECT_EQ(StrUtils::Utf8CharSizeAt("a", 0), 1);
    EXPECT_EQ(StrUtils::Utf8CharSizeAt("ą", 0), 2);
    EXPECT_EQ(StrUtils::Utf8CharSizeAt("中", 0), 3);
}

TEST(StringUtilTests, StringConversion)
{
    std::string text = u8",./;AaZzĄąĘę中";
    std::wstring expected = L",./;AaZzĄąĘę中";

    std::wstring unicode = StrUtils::Utf8StringToUnicode(text);
    std::string result = StrUtils::UnicodeStringToUtf8(unicode);

    EXPECT_EQ(result, text);
    EXPECT_EQ(unicode, expected);
}

TEST(StringUtilTests, ToLowerTest)
{
    std::string text = u8",./;AaBbĄąĘę中";
    std::string expected = u8",./;aabbąąęę中";

    auto result = StrUtils::ToLower(text);

    EXPECT_EQ(result, expected);
}

TEST(StringUtilTests, ToUpperTest)
{
    std::string text = u8",./;AaBbĄąĘę中";
    std::string expected = u8",./;AABBĄĄĘĘ中";

    auto result = StrUtils::ToUpper(text);

    EXPECT_EQ(result, expected);
}

} // namespace StringUtils
