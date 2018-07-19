<?php

// *****************************************************************************
//                            FilterComponent (base)

class FilterComponent
{
   public function getHtml()
   {
      // Override in child classes.
   }
   
   public function update()
   {
      // Override in child classes.
   }
   
   public function render()
   {
      echo (Filter::getHtml());
   }
}

// *****************************************************************************
//                            DateFilterComponent

class DateFilterComponent extends FilterComponent
{
   public $startDate;
   public $endDate;
   
   function __construct()
   {
      $this->startDate = Time::now("Y-m-d");
      $this->endDate = Time::now("Y-m-d");
   }
   
   public function getHtml()
   {
      $html =
<<<HEREDOC
      <div class="flex-horizontal filter-component">
         <div>Start Date:&nbsp</div>
         <input type="date" id="start-date-input" name="startDate" value="$this->startDate">
         <div>&nbsp</div>
         <div>End Date:&nbsp</div>
         <input type="date" id="end-date-input" name="endDate" value="$this->endDate">
      </div>
HEREDOC;

      return ($html);
   }
   
   public function update()
   {
      if (isset($_POST['startDate']))
      {
         $this->startDate = $_POST['startDate'];
      }
      
      if (isset($_POST['endDate']))
      {
         $this->endDate = $_POST['endDate'];
      }
   }
}

// *****************************************************************************
//                                  FilterButton

class FilterButton extends FilterComponent
{
   public function getHtml()
   {
      $html =
<<<HEREDOC
      <div><button class="mdl-button mdl-js-button mdl-button--raised filter-component">Filter</button></div>
HEREDOC;
      
      return ($html);
   }
}

// *****************************************************************************
//                                  TodayButton

class TodayButton extends FilterComponent
{
   public function getHtml()
   {
      $html = 
<<<HEREDOC
      <div><button class="mdl-button mdl-js-button mdl-button--raised filter-component" onclick="filterToday()">Today</button></div>
HEREDOC;

      return ($html);
   }
}

// *****************************************************************************
//                                  YesterdayButton

class YesterdayButton extends FilterComponent
{
   public function getHtml()
   {
      $html =
<<<HEREDOC
      <div><button class="mdl-button mdl-js-button mdl-button--raised filter-component" onclick="filterYesterday()">Yesterday</button></div>
HEREDOC;
      
      return ($html);
   }
}

// *****************************************************************************
//                                  ThisWeekButton

class ThisWeekButton extends FilterComponent
{
   public function getHtml()
   {
      $html =
<<<HEREDOC
      <div><button class="mdl-button mdl-js-button mdl-button--raised filter-component" onclick="filterThisWeek()">This Week</button></div>
HEREDOC;
      
      return ($html);
   }
}

// *****************************************************************************
//                                  FilterSpacer

class FilterSpacer extends FilterComponent
{
   public function getHtml()
   {
      $html =
<<<HEREDOC
      <div class="filter-component"></div>
HEREDOC;
      
      return ($html);
   }
}

class FilterDivider extends FilterComponent
{
   public function getHtml()
   {
      $html =
<<<HEREDOC
      <div class="filter-component">|</div>
HEREDOC;
      
      return ($html);
   }
}


// *****************************************************************************
//                                  Filter

class Filter
{
   public $components = array();
   
   public function __construct()
   {
   }
   
   public function add($component)
   {
      $this->components[] = $component;
   }
   
   public function addByName($key, $component)
   {
      $this->components[$key] = $component;
   }
   
   public function get($key)
   {
      return ($this->components[$key]);
   }
   
   public function update()
   {
      foreach($this->components as $component)
      {
         $component->update();
      }
   }
   
   public function getHtml()
   {
      $html =
<<<HEREDOC
      <!-- Use some kind of root path variable -->
      <link rel="stylesheet" type="text/css" href="/pptp/common/filter.css"/>
      <script src="/pptp/common/filter.js"></script>
      
      <form action="#" method="POST">
      <div class="flex-horizontal">
HEREDOC;

      foreach($this->components as $component)
      {
         $html .= $component->getHtml();
      }

      $html .=
<<<HEREDOC
      </div>
      </form>
HEREDOC;
      
      return ($html);
   }
   
   public function render()
   {
      echo (Filter::getHtml());
   }
}

/*
$operators = UserInfo::getUsersByRole(Role::OPERATOR);

$filter = new Filter();

$filter->add(new DateFilterComponent());
$filter->add(new UserFilterComponent("Operators", $operators, 1975, true));
$filter->add(new FilterButton());
$filter->add(new FilterDivider());
$filter->add(new TodayButton());
$filter->add(new YesterdayButton());
$filter->add(new ThisWeekButton());

$filter->render();
*/
?>