/*
 *+-----------------+
 *PROJECT TROPICANA
 *+-----------------+
 */

 //Remove unique identifier when document is going to client..for security
 exports.DBToClient = function(doc) {
     var result = JSON.parse(JSON.stringify(doc));
     delete result.nonce;

     return result;
 };
