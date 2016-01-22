/**
* Device.js
*
* @description :: TODO: You might write a short summary of how this model works and what it represents here.
* @docs        :: http://sailsjs.org/#!documentation/models
*/

module.exports = {

  attributes: {
  	type: {
  		type:'integer',
  		required: true
  	},
  	id: {
  		type:'integer',
  		required: true
  	},
    state:{
      type: 'integer'
    },
  	owner: {
  		model:'sigfox',
      required: true
  	}

  }
};

