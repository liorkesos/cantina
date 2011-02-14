<?php
// $Id: balance_tracker.views.inc,v 1.1 2009/07/20 14:31:27 brianV Exp $
/**
 * @file
 * Provide views data and handlers for balance_tracker.module
 */
 
 /**
 * Implementation of hook_views_data()
 */
function balance_tracker_views_data() {
  // ----------------------------------------------------------------
  // balance_items table

  $data['balance_items']['table']['group']  = t('Balances');

  $data['balance_items']['table']['join'] = array(
    // ...to the user table
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  
  $data['balance_items']['table']['base'] = array(
    'field' => 'bid',
    'title' => t('Balance item'),
    'help' => t("Balances are amounts credited or debited to a user."),
  );
  
// bid
  $data['balance_items']['bid'] = array(
    'title' => t('Balance ID'),
    'help' => t('The unique identifier for this balance item.'),
		'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // user
  $data['balance_items']['uid'] = array(
    'title' => t('User'),
    'help' => t('The ID of the user this item belongs to.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
     'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
     ),
  );
  
    // timestamp
  $data['balance_items']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('The timestamp this item was created.'),

    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
   // type
  $data['balance_items']['type'] = array(
    'title' => t('Type'),
    'help' => t('Whether this item is a credit or debit.'),
    
		'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'balance_tracker_handler_filter_balance_items_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // message
  $data['balance_items']['message'] = array(
    'title' => t('Message'),
    'help' => t('Log message related to this item.'),
		'field' => array(
      'handler' => 'views_handler_field_xss',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // amount
  $data['balance_items']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('The amount credited or debited with this transaction.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // balance
  $data['balance_items']['balance'] = array(
    'title' => t('Balance'),
    'help' => t('The balance after a transaction has taken place.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function balance_tracker_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'balance_tracker') . '/includes',
    ),
    'handlers' => array(
      'balance_tracker_handler_filter_balance_items_type' => array(
        'parent' =>'views_handler_filter_in_operator',
      ),
    ),
  );
}

