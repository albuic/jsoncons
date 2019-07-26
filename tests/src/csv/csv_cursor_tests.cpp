// Copyright 2018 Daniel Parker
// Distributed under Boost license

#if defined(_MSC_VER)
#include "windows.h" // test no inadvertant macro expansions
#endif
#include <jsoncons/json.hpp>
#include <jsoncons/json_encoder.hpp>
#include <jsoncons_ext/csv/csv_cursor.hpp>
#include <jsoncons_ext/csv/csv.hpp>
#include <catch/catch.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>

using namespace jsoncons;

TEST_CASE("csv_cursor n_rows test")
{
    const std::string data = R"(index_id,observation_date,rate
EUR_LIBOR_06M,2015-10-23,0.0000214
EUR_LIBOR_06M,2015-10-26,0.0000143
EUR_LIBOR_06M,2015-10-27,0.0000001
)";

    SECTION("test 1")
    {
        csv::csv_options options;
        options.assume_header(true)
               .mapping(csv::mapping_type::n_rows);
        csv::csv_cursor cursor(data, options);

        CHECK(cursor.current().event_type() == staj_event_type::begin_array);
        cursor.next();

        CHECK(cursor.current().event_type() == staj_event_type::begin_array);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        CHECK(cursor.current().get<std::string>() == std::string("index_id"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        CHECK(cursor.current().get<std::string>() == std::string("observation_date"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        CHECK(cursor.current().get<std::string>() == std::string("rate"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::end_array);
        cursor.next();

        CHECK(cursor.current().event_type() == staj_event_type::begin_array);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::double_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::end_array);
        cursor.next();

        CHECK(cursor.current().event_type() == staj_event_type::begin_array);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::double_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::end_array);
        cursor.next();

        CHECK(cursor.current().event_type() == staj_event_type::begin_array);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::double_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::end_array);
        cursor.next();

        CHECK(cursor.current().event_type() == staj_event_type::end_array);
        cursor.next();
        CHECK(cursor.done());
    }
}

TEST_CASE("csv_cursor n_objects test")
{
    const std::string data = R"(index_id,observation_date,rate
EUR_LIBOR_06M,2015-10-23,0.0000214
EUR_LIBOR_06M,2015-10-26,0.0000143
EUR_LIBOR_06M,2015-10-27,0.0000001
)";

    SECTION("test 2")
    {
        csv::csv_options options;
        options.assume_header(true)
               .mapping(csv::mapping_type::n_objects);
        csv::csv_cursor cursor(data, options);

        for (; !cursor.done(); cursor.next())
        {
            const auto& event = cursor.current();
            switch (event.event_type())
            {
                case staj_event_type::begin_array:
                    std::cout << event.event_type() << " " << "\n";
                    break;
                case staj_event_type::end_array:
                    std::cout << event.event_type() << " " << "\n";
                    break;
                case staj_event_type::begin_object:
                    std::cout << event.event_type() << " " << "\n";
                    break;
                case staj_event_type::end_object:
                    std::cout << event.event_type() << " " << "\n";
                    break;
                case staj_event_type::name:
                    // Or std::string_view, if supported
                    std::cout << event.event_type() << ": " << event.get<jsoncons::string_view>() << "\n";
                    break;
                case staj_event_type::string_value:
                    // Or std::string_view, if supported
                    std::cout << event.event_type() << ": " << event.get<jsoncons::string_view>() << "\n";
                    break;
                case staj_event_type::null_value:
                    std::cout << event.event_type() << "\n";
                    break;
                case staj_event_type::bool_value:
                    std::cout << event.event_type() << ": " << std::boolalpha << event.get<bool>() << "\n";
                    break;
                case staj_event_type::int64_value:
                    std::cout << event.event_type() << ": " << event.get<int64_t>() << "\n";
                    break;
                case staj_event_type::uint64_value:
                    std::cout << event.event_type() << ": " << event.get<uint64_t>() << "\n";
                    break;
                case staj_event_type::double_value:
                    std::cout << event.event_type() << ": " << event.get<double>() << "\n";
                    break;
                default:
                    std::cout << "Unhandled event type: " << event.event_type() << " " << "\n";;
                    break;
            }
        }
/*
        CHECK(cursor.current().event_type() == staj_event_type::begin_array);
        cursor.next();

        CHECK(cursor.current().event_type() == staj_event_type::begin_object);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::name);
        CHECK(cursor.current().get<std::string>() == std::string("index_id"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::name);
        CHECK(cursor.current().get<std::string>() == std::string("observation_date"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::name);
        CHECK(cursor.current().get<std::string>() == std::string("rate"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::double_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::end_object);
        cursor.next();

        CHECK(cursor.current().event_type() == staj_event_type::begin_object);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::name);
        CHECK(cursor.current().get<std::string>() == std::string("index_id"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::name);
        CHECK(cursor.current().get<std::string>() == std::string("observation_date"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::name);
        CHECK(cursor.current().get<std::string>() == std::string("rate"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::double_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::end_object);
        cursor.next();

        CHECK(cursor.current().event_type() == staj_event_type::begin_object);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::name);
        CHECK(cursor.current().get<std::string>() == std::string("index_id"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::name);
        CHECK(cursor.current().get<std::string>() == std::string("observation_date"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::string_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::name);
        CHECK(cursor.current().get<std::string>() == std::string("rate"));
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::double_value);
        cursor.next();
        CHECK(cursor.current().event_type() == staj_event_type::end_object);
        cursor.next();

        CHECK(cursor.current().event_type() == staj_event_type::end_array);
        cursor.next();
        CHECK(cursor.done());
*/
    }
}
