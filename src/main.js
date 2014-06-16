(function() {
  var keyboard = require('bindings')('keyboard.node');

  module.exports = function(command, args, options) {
    return keyboard.keyboard(command, args, options);
  };
}).call(this);
