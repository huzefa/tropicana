/*
 *+-----------------+
 *PROJECT TROPICANA
 *+-----------------+
 */

var express = require('express'),
        app = express(),
         me = process.env.CLOUDANT_USERNAME,
   password = process.env.CLOUDANT_PASSWORD,
   Cloudant = require('cloudant')({account:me, password:password}),
         db = Cloudant.db.use("messages"),
 bodyParser = require ('body-parser'),
 DocumentModifier = require('./DocumentModifier');

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

/*
 * TODO: MOVE THIS SHT SOMEWHERE VALUABLE
*/
app.route('/tropicana')
  .get(function(req, res) {
    db.get(req.headers.key, function(err, data) {
     if (err)
        return console.log('Message Retrieval error:', err.message);

      console.log('client has requested to view the Message.');

      var modifiedData = DocumentModifier.DBToClient(data);
      res.send(modifiedData);
    });
  })
  .post(function(req, res) {
      //TODO: Create unique identifier for client and make it an indentifier(instead of nonce)
      //TODO: Validate the json coming in..need z-schema validator maybe
    db.insert(req.body, req.body.nonce, function(err, body, header) {
      if (err)
        return console.log('Message Insertion error:', err.message);

      console.log('client has inserted the Message.');
      res.send(body);
    });
  });

var server = app.listen(3000, '0.0.0.0', function() {
  console.log('Listening on port %d', server.address().port);
});
