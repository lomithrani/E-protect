  //library to connect to sailsjs websockets
  var socketIOClient = require('socket.io-client');
  //var sailsIOClient = require('sails.io.js');

  //library to do the tcp server
  var net = require('net');
        



  var server = net.createServer(function(tcpSocket) { //'connection' listener
          
    //socket was sucessfully connected
    console.log('client connected');

    var socket=socketIOClient.connect('http://localhost:1337', {
    'force new connection': true
});
    


    //notify on deconnection
    tcpSocket.on('end', function() {
      console.log('client disconnected');
      socket.emit('disconnect');
      socket.disconnect();
    });

   // Handle incoming messages from clients.
    tcpSocket.on('data', function (data) {

try{
      //if data is PING respond PONG
      if(data.toString('utf8', 0, 4)=='PING'){
        // console.log('I was pinged');
        tcpSocket.write('PONG\r\n'); 
      }
      else{
      
      var JSONdata = JSON.parse(data.toString('utf8', 0, data.length));
         
      //console.log(data.toString('utf8', 0, data.length));



      switch(JSONdata.cmd){
          
        case 'connect'://sigfox
          socket.emit('connect',JSONdata);
          // console.log('connect');
          break;
        case 'g'://statut , ty=type m, st=statut, id=id
          socket.emit('update',JSONdata);
          //console.log('g');
          break;
        case 'l'://lock 
          socket.emit('lock',JSONdata);
          // console.log('l');
          break;
        case 'u'://unlock
          socket.emit('unlock',JSONdata);
          // console.log('u');
          break;
 
      }

    } 
  }catch (ex) {
    console.log('Exception:'+ex);
  }
      
    });

  });






  server.listen(8124, function() { //'listening' listener
    

          console.log('linstening port 8124');


  });