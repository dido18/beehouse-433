var express = require('express');
var path = require('path');

module.exports = function(http, io){

  // http.use(express.static(__dirname + '/node_modules'));
  //
  http.get('/', function(req, res,next) {
      res.sendFile(path.resolve('www/views/index.html'));//__dirname + '/../www/views/index.html'); // __dirname +
  });
  // http.get(['/', '/index.html'], function(req, res){
	// 	res.render('index.html');
	// });

  // serve as static all the other routes
	// var web_dir = path.resolve(__dirname, '../www');
	// http.get('*', express.static(web_dir));
};
