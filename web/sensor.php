<?php

require_once 'common/database.php';
require_once 'common/time.php';

function recordDetection($gnomeId)
{
   $database = new GnomeDatabase();
   
   $database->connect();
   
   if ($database->isConnected())
   {
      $database->newDetection($gnomeId, Time::now("Y-m-d h:i:s A"));
   }
}

// *****************************************************************************
//                                   Begin

if (isset($_GET["gnomeId"]) && (isset($_GET["action"])))
{
   $gnomeId = $_GET["gnomeId"];
   $action = $_GET["action"];
   
   switch ($action)
   {
      case "detection":
      {
         recordDetection($gnomeId);
         break;
      }
         
      default:
      {
         break;
      }
   }
}

?>