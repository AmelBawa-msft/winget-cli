// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
#include "pch.h"
#include "TestCommon.h"
#include <Argument.h>

using namespace AppInstaller::CLI;
using namespace std::string_view_literals;

TEST_CASE("EnsureAllArgumentsDefined", "[argument]")
{
    using Arg_t = std::underlying_type_t<Execution::Args::Type>;
    for (Arg_t i = static_cast<Arg_t>(0); i < static_cast<Arg_t>(Execution::Args::Type::Max); ++i)
    {
        REQUIRE_NOTHROW(ArgumentCommon::ForType(static_cast<Execution::Args::Type>(i)));
    }
}

TEST_CASE("ArgumentLogRedactsSensitiveValues", "[argument]")
{
    auto type = GENERATE(Execution::Args::Type::Proxy, Execution::Args::Type::SettingValue,
        Execution::Args::Type::Word, Execution::Args::Type::CommandLine);
    auto value = GENERATE(std::string{ "http://log-user:log-password" } + '@' + "localhost", std::string{ "unfinished-credential" });
    Execution::Args args;
    args.AddArg(type, value);

    std::string log = ArgumentCommon::GetLogString(args);

    REQUIRE(log.find("[redacted]") != std::string::npos);
    REQUIRE(log.find(value) == std::string::npos);
    REQUIRE(args.GetArg(type) == value);
}

TEST_CASE("ArgumentLogPreservesNonSensitiveValues", "[argument]")
{
    Execution::Args args;
    args.AddArg(Execution::Args::Type::Query, "public@example.test"sv);
    args.AddArg(Execution::Args::Type::Source, "test source"sv);
    args.AddArg(Execution::Args::Type::Exact);

    std::string log = ArgumentCommon::GetLogString(args);

    REQUIRE(log.find(" --query 'public@example.test'") != std::string::npos);
    REQUIRE(log.find(" --source 'test source'") != std::string::npos);
    REQUIRE(log.find(" --exact") != std::string::npos);
    REQUIRE(args.GetArg(Execution::Args::Type::Query) == "public@example.test");
}