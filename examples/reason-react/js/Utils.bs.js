// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Json = require("@glennsl/bs-json/src/Json.bs.js");
var Constant = require("./Constant.bs.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var Json_decode = require("@glennsl/bs-json/src/Json_decode.bs.js");
var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");

function uuid(param) {
  var uuid$1 = "";
  for(var i = 0; i <= 32; ++i){
    var random = Math.random() * 16;
    if (i === 8 || i === 12 || i === 16 || i === 20) {
      uuid$1 = uuid$1 + "-";
    }
    var match = i === 12;
    var tmp;
    if (match) {
      tmp = 4;
    } else {
      var match$1 = i === 16;
      tmp = match$1 ? random & 3 | 8 : random;
    }
    uuid$1 = uuid$1 + tmp.toString();
  }
  return uuid$1;
}

function sanatizeTodo(text) {
  return text.trim();
}

function filterTodos(todos, filter) {
  return Belt_Array.keep(todos, (function (todo) {
                switch (filter) {
                  case /* All */0 :
                      return true;
                  case /* Active */1 :
                      return !todo[/* completed */2];
                  case /* Completed */2 :
                      return todo[/* completed */2];
                  
                }
              }));
}

function valueFromEvent($$event) {
  return $$event.target.value;
}

function booleanValueFromEvent($$event) {
  return $$event.target.checked;
}

function pluralize(count, word) {
  var match = count === 1;
  if (match) {
    return word;
  } else {
    return word + "s";
  }
}

function decodeTodo(json) {
  return /* record */[
          /* id */Json_decode.field("id", Json_decode.string, json),
          /* title */Json_decode.field("title", Json_decode.string, json),
          /* completed */Json_decode.field("completed", Json_decode.bool, json)
        ];
}

function decodeTodos(param) {
  return Json_decode.array(decodeTodo, param);
}

var Decoder = {
  decodeTodo: decodeTodo,
  decodeTodos: decodeTodos
};

function encodeTodo(todo) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "id",
                todo[/* id */0]
              ],
              /* :: */[
                /* tuple */[
                  "title",
                  todo[/* title */1]
                ],
                /* :: */[
                  /* tuple */[
                    "completed",
                    todo[/* completed */2]
                  ],
                  /* [] */0
                ]
              ]
            ]);
}

function encodeTodos(todos) {
  return Json.stringify(todos.map(encodeTodo));
}

var Encoder = {
  encodeTodo: encodeTodo,
  encodeTodos: encodeTodos
};

function getTodos(param) {
  var strTodos = localStorage.getItem(Constant.storageKey);
  var jsonTodos = Belt_Option.getWithDefault(strTodos === null ? undefined : Caml_option.some(strTodos), "[]");
  var param$1 = Json.parseOrRaise(jsonTodos);
  return Json_decode.array(decodeTodo, param$1);
}

function saveTodos(todos) {
  localStorage.setItem(Constant.storageKey, Json.stringify(todos.map(encodeTodo)));
  return /* () */0;
}

var Store = {
  getTodos: getTodos,
  saveTodos: saveTodos
};

exports.uuid = uuid;
exports.sanatizeTodo = sanatizeTodo;
exports.filterTodos = filterTodos;
exports.valueFromEvent = valueFromEvent;
exports.booleanValueFromEvent = booleanValueFromEvent;
exports.pluralize = pluralize;
exports.Decoder = Decoder;
exports.Encoder = Encoder;
exports.Store = Store;
/* No side effect */