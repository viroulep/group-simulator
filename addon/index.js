'use strict'

const EventEmitter = require('events').EventEmitter
const addon = require('bindings')('emit_from_cpp')

// 20 people doing 12 avg at 333
let result = addon.RunGroup("333", 20, 12);
let minutes = Math.floor(result/60);
let sec = result - 60 * minutes;
console.log("This is the result:", minutes, " minutes ", sec);
