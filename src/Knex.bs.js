'use strict';

var Js_null_undefined = require("bs-platform/lib/js/js_null_undefined.js");

function fromTable(string, knex) {
  return knex(string);
}

function insert($staropt$star, data) {
  var returning = $staropt$star ? $staropt$star[0] : /* Some */["*"];
  var partial_arg = Js_null_undefined.fromOption(returning);
  return (function (param) {
      return param.insert(data, partial_arg);
    });
}

function update($staropt$star, data) {
  var returning = $staropt$star ? $staropt$star[0] : /* Some */["*"];
  var partial_arg = Js_null_undefined.fromOption(returning);
  return (function (param) {
      return param.update(data, partial_arg);
    });
}

function del($staropt$star, data) {
  var returning = $staropt$star ? $staropt$star[0] : /* Some */["*"];
  var partial_arg = Js_null_undefined.fromOption(returning);
  return (function (param) {
      return param.del(data, partial_arg);
    });
}

var $$delete = del;

exports.fromTable = fromTable;
exports.insert = insert;
exports.update = update;
exports.del = del;
exports.$$delete = $$delete;
/* No side effect */
