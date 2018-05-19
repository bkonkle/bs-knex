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

describe("Knex", (function () {
        describe("queryBuilder", (function () {
                var knex = Knex$1(config);
                Jest.test("returns the correct sql", (function () {
                        var sql = Knex.fromTable("users", knex).where({
                                id: "awesome"
                              }).toSQL();
                        return Jest.ExpectJs[/* toEqual */12]("select * from `users` where `id` = ?", Jest.ExpectJs[/* expect */0](sql.sql));
                      }));
                return Jest.test("returns the correct bindings", (function () {
                              var sql = Knex.fromTable("users", knex).where({
                                      id: "awesome"
                                    }).toSQL();
                              return Jest.ExpectJs[/* toEqual */12](/* array */["awesome"], Jest.ExpectJs[/* expect */0](sql.bindings));
                            }));
              }));
        return /* () */0;
      }));

exports.connection = connection;
exports.config = config;
/*  Not a pure module */
