(function() {
  var keyboard = require('bindings')('keyboard.node');

  module.exports.foo = keyboard.foo;
}).call(this);
