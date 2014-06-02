#include <fost/aws>
#include <fost/jsondb>
#include <varanus/acanthurus.hpp>
#include <varanus/obor.hpp>


namespace {


    const fostlib::setting<fostlib::string> c_account(
        "proxy-lib/uploader.cpp",
        fostlib::aws::s3::bucket::s_account_name, "wos");
    const fostlib::setting<fostlib::string> c_bucket(
        "proxy-lib/uploader.cpp",
        "Uploader", "bucket", "fbwc2014", true);


    fostlib::json upload_host_db(const fostlib::json &config) {
        fostlib::aws::s3::bucket bucket(
            fostlib::coerce<fostlib::ascii_printable_string>(
                c_bucket.value()));

        fostlib::jsondb::local acanthurus(varanus::acanthurus_config());
        fostlib::string this_host(
            fostlib::coerce<fostlib::string>(
                acanthurus["hostname"]));

        fostlib::string host(fostlib::coerce<
            fostlib::nullable<fostlib::string>>(config["hostname"])
                .value("localhost"));
        varanus::db hostdb(varanus::host_data(host));
        fostlib::jsondb::local hostdata(hostdb());

        fostlib::string content(fostlib::json::unparse(
            hostdata[fostlib::jcursor()], true));
        boost::filesystem::wpath to(
            boost::filesystem::wpath("/app") /
            fostlib::coerce<boost::filesystem::wpath>(this_host) /
            "db" / "host" /
            fostlib::coerce<boost::filesystem::wpath>(host) /
            fostlib::coerce<boost::filesystem::wpath>(
                fostlib::timestamp::now()));
        to.replace_extension(".json");
        bucket.put(content, to);

        return fostlib::json();
    }
    const varanus::obor c_statistics("proxy.uploader.db.host", upload_host_db);


}

