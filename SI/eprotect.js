var instructionsNouveauVisiteur = function(req, res) {
  var url = require('url');
  var querystring = require('querystring');
  var uri = url.parse(req.url);
  mongojs = require("mongojs");
  db = mongojs.connect("localhost:27017", ["eprotectDb"]);
  
  db.clients.find({"nom": "gentil"},function(err, records) {
  
  var html = 'test des reponses';
  });
  
  
  res.writeHead(200);
  res.end(''+records[0].prenom);
}
var http = require('http'); 
var server = http.createServer(instructionsNouveauVisiteur);
server.listen(80);
