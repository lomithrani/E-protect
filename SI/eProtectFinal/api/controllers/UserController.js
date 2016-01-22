/**
 * UserController
 *
 * @description :: Server-side logic for managing users
 * @help        :: See http://links.sailsjs.org/docs/controllers
 */

var bcrypt = require('bcrypt');
 module.exports = {


  'new': function(req,res){

    res.view();

  },
  // render the profile view (e.g. /views/show.ejs)
  show: function(req, res, next) {
    var id=0;
   
    if(req.param('id') == undefined)
      id = req.session.User.id;
    
      
    else id=req.param('id');
    User.findOne(id, function foundUser(err, user) {
      if (err) return next(err);
      if (!user) return next();
      res.view({
        user: user
      });
    });
  },

  index: function(req, res, next) {

    // Get an array of all users in the User collection(e.g. table)
    User.find(function foundUsers(err, users) {
      if (err) return next(err);
      // pass the array down to the /views/index.ejs page
      res.view({
        users: users
      });
    });
  },
    // render the edit view (e.g. /views/edit.ejs)
  edit: function(req, res, next) {

    // Find the user from the id passed in via params
    User.findOne(req.param('id'), function foundUser(err, user) {
      if (err) return next(err);
      if (!user) return next('User doesn\'t exist.');

      res.view({
        user: user
      });
    });
  },

  // process the info from edit view
  update: function(req, res, next) {
var admin=0;
    
      if(req.param('isAdmin') == 'checked')
        admin = 0;
      else
        admin = 1;

 User.findOne(req.param('id'), function foundUser(err, user) {
    if (req.session.User.isAdmin == 2) {

      if(user.isAdmin == 2)
        admin = 2;
      var userObj = {
        firstName: req.param('firstName'),
        lastName: req.param('lastName'),
        email: req.param('email'),
        tel: req.param('tel'),
        street: req.param('street'),
        city: req.param('city'),
        postCode: req.param('postCode'),
        isAdmin: admin
      }
    } else {
      if((req.session.User.isAdmin == 1)||(req.session.User.id == user.id))
      {
      var userObj = {
        firstName: req.param('firstName'),
        lastName: req.param('lastName'),
        email: req.param('email'),
        tel: req.param('tel'),
        street: req.param('street'),
        city: req.param('city'),
        postCode: req.param('postCode'),
        sigfox: req.param('sigfox')
      }
    }
      else {
        var userObj = {}
      }
    }


       User.update(req.param('id'), userObj, function userUpdated(err,user) {
      if (err) {
        return res.redirect('/user/edit/' + req.param('id'));
      }
           
     });

  

      res.redirect('/user/show/' + req.param('id'));
    });
  },


  create: function(req, res, next){
    var params = req.params.all();
   
    params.passwd = bcrypt.hashSync(params.passwd,10);

    params.isAdmin = 0;

    User.create(params).exec(function createCB(err,user){

      if(err) 
      {

        req.session.flash = { 
          err: err
        }
         res.redirect('/user/new'); 
      }
      if(user)
      {
      // Log user in
      req.session.authenticated = true;
      req.session.User = user;
      console.log('User.publish create');

    User.publishCreate({id:user.id,firstName:user.name});
            


      res.redirect('/user/show/' + user.id);
    }
      req.session.flash = {};
    });



},


 connect: function(req, res, next){
  var params = req.params.all();

      // Find the base from the id passed in via params
    Sigfox.findOne({ where: {sigfoxId: req.param('sigfoxId')}}, function foundUser(err, sigfox) {
      if (err) return next(err);
      if (!sigfox) return next('Base doesnt exist');
      

      if (sigfox.owner) { 
        console.log(sigfox.owner);
       res.redirect('/user/'+req.session.User.id);
      }
        else {
          console.log('lets connect');
          sigfox.owner = req.session.User.id;
          Sigfox.update({ where: {sigfoxId: req.param('sigfoxId')}}, sigfox, function userUpdated(err) {
       if (err)  res.redirect('/user/' + req.session.User.id);      
      });
    }

});

},

 subscribe: function(req, res) {
 
    // Find all current users in the user model
    User.find(function foundUsers(err, users) {
      if (err) return next(err);
 
      // subscribe this socket to the user instance rooms
      User.subscribe(req.socket, users);
      User.watch(req);
  console.log('User with socket id '+req.socket.id+' is now subscribed to all of the model instances in \'users\'.');
      
      // This will avoid a warning from the socket for trying to render
      // html over the socket.
      res.send(200);
    });
  }

};

