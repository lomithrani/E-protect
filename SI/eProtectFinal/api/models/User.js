/**
* User.js
*
* @description :: TODO: You might write a short summary of how this model works and what it represents here.
* @docs        :: http://sailsjs.org/#!documentation/models
*/

module.exports = {

  schema :true,
  attributes: {

  	lastName : { 
  		type: 'string',
  		required: true
  	 },
  	firstName : { 
  		type: 'string',
  		required: true
  	},
  	email : { 
  		type: 'string',
  		email: true,
  		required: true,
      unique: true
  	},
  	passwd : { 
  		type: 'string',
  		required: true
  	},
  	tel : { 
      type: 'string',
      required: true
  	},
  	sigfox : {
      collection: 'sigfox',
      via: 'owner'
  	},
  	street : { 
      type: 'string',
      required: true
  	},
    city : { 
      type: 'string',
      required:true
    },
    postCode:{
      type: 'string',
      required: true
    },
    isAdmin : {
      type: 'integer',
      defaultsTo: 0,
    },
    online: {
      type: 'boolean',
      defaultsTo: false
    },

    toJSON: function(){
        var obj= this.toObject();
        delete obj.passwd;
        delete obj._csrf
        return obj;


    }
	
  }
};

