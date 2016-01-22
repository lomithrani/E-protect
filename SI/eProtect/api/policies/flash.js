/**
 * sessionAuth
 *
 * @module      :: Policy
 * @description :: Simple policy to allow any authenticated user
 *                 Assumes that your login action in one of your controllers sets `req.session.authenticated = true;`
 * @docs        :: http://sailsjs.org/#!documentation/policies
 *
 */
module.exports = function(req, res, next) {
 res.locals.flash = {};

 if(!req.session.flash) return next();

 res.locals.flash = _.clone(req.session.flash);

 // clear flash
 req.session.flash = {};

 next();
};
