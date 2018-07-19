<?php

require_once 'databaseKey.php';
require_once 'time.php';

interface Database
{
   public function connect();

   public function disconnect();

   public function isConnected();

   public function query(
      $query);
}

class MySqlDatabase implements Database
{
   function __construct(
      $server,
      $user,
      $password,
      $database)
   {
      $this->server = $server;
      $this->user = $user;
      $this->password = $password;
      $this->database = $database;
   }

   public function connect()
   {
      // Create connection
      $this->connection = new mysqli($this->server, $this->user, $this->password, $this->database);

      // Check connection
      if ($this->connection->connect_error)
      {
         // TODO?
      }
      else
      {
         $this->isConnected = true;
      }
   }

   public function disconnect()
   {
      if ($this->isConnected())
      {
         $this->connection->close();
      }
   }

   public function isConnected()
   {
      return ($this->isConnected);
   }

   public function query(
      $query)
   {
      $result = NULL;

      if ($this->isConnected())
      {
         $result = $this->connection->query($query);
      }
      
      return ($result);
   }
   
   protected function getConnection()
   {
      return ($this->connection);
   }

   private $server = "";

   private $user = "";

   private $password = "";

   private $database = "";

   private $connection;

   private $isConnected = false;
}

class GnomeDatabase extends MySqlDatabase
{
  
   public function __construct()
   {
      global $SERVER, $USER, $PASSWORD, $DATABASE;
      
      parent::__construct($SERVER, $USER, $PASSWORD, $DATABASE);
   }
   
   public function newDetection(
      $gnomeId,
      $dateTime)
   {
      $sqlDateTime = Time::toMySqlDate($dateTime);
      
      $query = "INSERT INTO detection (gnomeId, dateTime) VALUES ('$gnomeId', '$sqlDateTime');";

      $result = $this->query($query);

      return ($result);
   }
   
   public function getDetections(
      $gnomeId,
      $startDate,
      $endDate)
   {
      $result = NULL;
      if ($gnomeId == "")
      {
         $query = "SELECT * FROM detection WHERE dateTime BETWEEN '" . Time::toMySqlDate($startDate) . "' AND '" . Time::toMySqlDate($endDate) . "' ORDER BY dateTime DESC;";
         $result = $this->query($query);
      }
      else
      {
         $query = "SELECT * detection WHERE gnomeId=" . $gnomeId . " AND dateTime BETWEEN '" . Time::toMySqlDate($startDate) . "' AND '" . Time::toMySqlDate($endDate) . "' ORDER BY dateTime DESC;";
         $result = $this->query($query);
      }
      
      return ($result);
   }
}

?>
