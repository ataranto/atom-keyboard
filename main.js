(function() {
  var keyboard = require('bindings')('keyboard.node');
  module.exports.setCallback = keyboard.setCallback;
}).call(this);
