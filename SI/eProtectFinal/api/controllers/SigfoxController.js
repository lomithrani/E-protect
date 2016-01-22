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
    
  },

    
  subscribe: function(req, res) {
 

 console.log('looking for owner'+ req.session.User.id);
    // Find all 
    Sigfox.find({ where: {owner: req.session.User.id}},function foundSigfox(err, sigfoxes) {
      if (err) return next(err);

        Sigfox.subscribe(req.socket,sigfoxes);

        for(i in sigfoxes)
        {
            
             Device.find({ where: {owner: sigfoxes[i].id}},function foundDevices(err, devices) {

                Device.subscribe(req.socket,devices);

             });
        }
        

    });
 
      // This will avoid a warning from the socket for trying to render
      // html over the socket.
      res.send(200);
  }


};

