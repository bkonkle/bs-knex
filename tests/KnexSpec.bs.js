'use strict';

var Jest = require("@glennsl/bs-jest/src/jest.js");
var Knex = require("../src/Knex.bs.js");
var Knex$1 = require("knex");

var connection = {
  filename: ":memory:"
};

var config = {
  client: "sqlite3",
  connection: connection
};

Jest.describe("Knex", (function (param) {
        return Jest.describe("queryBuilder", (function (param) {
                      var knex = Knex$1(config);
                      Jest.test("returns the correct sql", (function (param) {
                              var sql = Knex.fromTable("users", knex).where({
                                      id: "awesome"
                                    }).toSQL();
                              return Jest.ExpectJs[/* toEqual */12]("select * from `users` where `id` = ?", Jest.ExpectJs[/* expect */0](sql.sql));
                            }));
                      return Jest.test("returns the correct bindings", (function (param) {
                                    var sql = Knex.fromTable("users", knex).where({
                                            id: "awesome"
                                          }).toSQL();
                                    return Jest.ExpectJs[/* toEqual */12](/* array */["awesome"], Jest.ExpectJs[/* expect */0](sql.bindings));
                                  }));
                    }));
      }));

exports.connection = connection;
exports.config = config;
/*  Not a pure module */
