/**
* Sigfox.js
*
* @description :: TODO: You might write a short summary of how this model works and what it represents here.
* @docs        :: http://sailsjs.org/#!documentation/models
*/

module.exports = {

  attributes: {
 	 sigfoxId: {
  		type: 'string',
  		unique: true,
      required: true
  	},
  	 devices: {
  	 	collection: 'device',
  	 	via: 'owner'
  	},
  	owner: {
  		model:'user'
  	},
    online: {
      type: 'boolean',
      defaultsTo: false
    }

  }
};

