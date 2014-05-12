#include <fost/http>
#include <fost/http.server.hpp>
#include <fost/file>
#include <fost/jsondb>


namespace proxy {


    /// Location for the cache data
    extern const fostlib::setting<fostlib::string> c_cache_dir;


    /// Return the root path for the cache
    boost::filesystem::wpath root();


    /// Return the cache address hash for the request
    fostlib::hex_string hash(const fostlib::http::server::request &req);


    /// Flush the entire cache
    void flush_cache();


    /// Returns the cache database (if we already know the root location)
    fostlib::jsondb &cache_db(const boost::filesystem::wpath &);

    /// Returns the cache database
    inline fostlib::jsondb &cache_db() {
        return cache_db(root());
    }


    /// Returns the cache entry for the hash
    fostlib::json db_entry(const fostlib::hex_string &hash);


}

