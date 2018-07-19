<?php

require_once 'common/database.php';
require_once 'common/filter.php';

// *****************************************************************************
//                                   ViewDetections

class ViewDetections
{
   private $filter;
   
   public function __construct()
   {
      if (isset($_SESSION["filter"]))
      {
         $this->filter = $_SESSION["filter"];
      }
      else
      {
         $this->filter = new Filter();
      
         $this->filter->addByName('date', new DateFilterComponent());
         $this->filter->add(new FilterButton());
         $this->filter->add(new FilterDivider());
         $this->filter->add(new TodayButton());
         $this->filter->add(new YesterdayButton());
         $this->filter->add(new ThisWeekButton());
      }
      
      $this->filter->update();
      
      $_SESSION["filter"] = $this->filter;
   }

   public function getHtml()
   {
      $filterDiv = ViewDetections::filterDiv();
      
      $detectionsDiv = ViewDetections::detectionsDiv();
      
      $html = 
<<<HEREDOC
      <div class="flex-vertical card-div">
         <div class="card-header-div">View Detections</div>
         <div class="flex-vertical content-div" style="justify-content: flex-start; height:400px;">
   
               $filterDiv
   
               $detectionsDiv
         
         </div>

      </div>
HEREDOC;
      
      return ($html);
   }
   
   public function render()
   {
      echo (ViewDetections::getHtml());
   }
      
   private function filterDiv()
   {
      return ($this->filter->getHtml());
   }
   
   private function detectionsDiv()
   {
      $html = 
<<<HEREDOC
         <div class="jobs-div">
            <table class="job-table">
               <tr>
                  <th>Gnome</th>                 
                  <th>Date</th>
                  <th>Time</th>
               </tr>
HEREDOC;
      
      $database = new GnomeDatabase();
      
      $database->connect();
      
      if ($database->isConnected())
      {
         // Start date.
         $startDate = new DateTime($this->filter->get('date')->startDate, new DateTimeZone('America/New_York'));  // TODO: Function in Time class
         $startDateString = $startDate->format("Y-m-d");
         
         // End date.
         // Increment the end date by a day to make it inclusive.
         $endDate = new DateTime($this->filter->get('date')->endDate, new DateTimeZone('America/New_York'));
         $endDate->modify('+1 day');
         $endDateString = $endDate->format("Y-m-d");
         
         $result = $database->getDetections("", $startDateString, $endDateString);
         
         if ($result)
         {
            while ($row = $result->fetch_assoc())
            {
               
               $dateStr = Time::fromMySqlDate($row["dateTime"], "m-d-Y");
               $timeStr = Time::fromMySqlDate($row["dateTime"], "h:i:s A");
                  
               $html .=
<<<HEREDOC
                  <tr>
                     <td>{$row["gnomeId"]}</td>
                     <td>$dateStr</td>
                     <td>$timeStr</td>
                  </tr>
HEREDOC;
            }
         }
      }
      
      $html .=
<<<HEREDOC
            </table>
         </div>
HEREDOC;
      
      return ($html);
   }
}

$page = new ViewDetections();
$page->render();
?>