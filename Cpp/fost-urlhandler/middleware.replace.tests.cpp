/**
    Copyright 2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/urlhandler>
#include <fost/push_back>
#include <fost/test>


FSL_TEST_SUITE(middleware_replace);


FSL_TEST_FUNCTION(nop) {
    fostlib::json config;
    fostlib::insert(config, "view", "fost.middleware.replace");
    fostlib::insert(config, "configuration", "view", "fost.response.200");
    fostlib::http::server::request req;
    auto response = fostlib::urlhandler::view::execute(
            config, "", req, fostlib::host{});
    FSL_CHECK_EQ(response.second, 200);
}


FSL_TEST_FUNCTION(simple) {
    fostlib::json config;
    fostlib::insert(config, "view", "fost.middleware.replace");
    fostlib::insert(config, "configuration", "view", "fost.response.200");
    fostlib::insert(config, "configuration", "replace", "OK", "Very OK");
    fostlib::http::server::request req;
    auto response = fostlib::urlhandler::view::execute(
            config, "", req, fostlib::host{});
    FSL_CHECK_EQ(response.second, 200);
    FSL_CHECK_EQ(
            response.first->body_as_string(),
            "<html><head><title>Very OK</title></head><body><h1>Very "
            "OK</h1></body></html>");
}


FSL_TEST_FUNCTION(setting) {
    fostlib::setting<fostlib::string> const s{"middleware.replace.tests.cpp",
                                              "Test middleware", "Replacement",
                                              "Very OK", true};
    fostlib::json config;
    fostlib::insert(config, "view", "fost.middleware.replace");
    fostlib::insert(config, "configuration", "view", "fost.response.200");
    fostlib::push_back(config, "configuration", "replace", "OK", "setting");
    fostlib::push_back(config, "configuration", "replace", "OK", s.section());
    fostlib::push_back(config, "configuration", "replace", "OK", s.name());
    fostlib::http::server::request req;
    auto response = fostlib::urlhandler::view::execute(
            config, "", req, fostlib::host{});
    FSL_CHECK_EQ(response.second, 200);
    FSL_CHECK_EQ(
            response.first->body_as_string(),
            "<html><head><title>Very OK</title></head><body><h1>Very "
            "OK</h1></body></html>");
}


FSL_TEST_FUNCTION(setting_with_default) {
    fostlib::json config;
    fostlib::insert(config, "view", "fost.middleware.replace");
    fostlib::insert(config, "configuration", "view", "fost.response.200");
    fostlib::push_back(config, "configuration", "replace", "OK", "setting");
    fostlib::push_back(
            config, "configuration", "replace", "OK", "Test middleware");
    fostlib::push_back(
            config, "configuration", "replace", "OK", "Not a setting value");
    fostlib::push_back(config, "configuration", "replace", "OK", "Default");
    fostlib::http::server::request req;
    auto response = fostlib::urlhandler::view::execute(
            config, "", req, fostlib::host{});
    FSL_CHECK_EQ(response.second, 200);
    FSL_CHECK_EQ(
            response.first->body_as_string(),
            "<html><head><title>Default</title></head><body><h1>Default</h1></"
            "body></html>");
}


FSL_TEST_FUNCTION(setting_with_default_object) {
    fostlib::json config;
    fostlib::insert(config, "view", "fost.middleware.replace");
    fostlib::insert(config, "configuration", "view", "fost.response.200");
    fostlib::push_back(config, "configuration", "replace", "OK", "setting");
    fostlib::push_back(
            config, "configuration", "replace", "OK", "Test middleware");
    fostlib::push_back(
            config, "configuration", "replace", "OK", "Not a setting value");
    fostlib::jcursor{"configuration", "replace", "OK"}.push_back(
            config, fostlib::json::object_t{});
    fostlib::http::server::request req;
    auto response = fostlib::urlhandler::view::execute(
            config, "", req, fostlib::host{});
    FSL_CHECK_EQ(response.second, 200);
    FSL_CHECK_EQ(
            response.first->body_as_string(),
            "<html><head><title>{}</title></head><body><h1>{}</h1></body></"
            "html>");
}
