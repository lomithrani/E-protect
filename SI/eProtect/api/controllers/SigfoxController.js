/**
 * SigfoxController
 *
 * @description :: Server-side logic for managing sigfoxes
 * @help        :: See http://links.sailsjs.org/docs/controllers
 */

module.exports = {
	
join: function(req, res) {
    

    var socket = req.socket;
    var io = sails.io;


    io.sockets.emit('user', {message: 'user tried to join'});

    res.json({
      success: true
    });
    
  }
};

