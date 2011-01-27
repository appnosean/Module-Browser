<?php 

/**
 * @file
 * 
 * Functions to work with csv files (read/write)
 */

define('ON_COLLISION_OVERWRITE', 1);
define('ON_COLLISION_SKIP'     , 2);
define('ON_COLLISION_ABORT'    , 3);

/**
 * Read CSV file
 *
 *  @param  string  $csv_file       the CSV file to read.
 *  @param  string  $sep_input      the character that is used in the CSV file as field separator.
 *  @param  string  $sep_index      the character that separates the fields used to construct the hash table's index.
 *  @param  array   $index_by       the array containing the columns to index the lookup table by, and the function to pre-process those columns with.
 *  @param  integer $on_collision       a constant that determines what to do when an index is already in use.
 *  @param  integer $rec_len        the maximum length of a record in the input file.
 *  @return mixed                   an error number or the resulting hash table. 
 *  
 */
function modulebrowser_read_lookup_table_from_csv( $csv_file, $separator_input = ';', $separator_index = '|', $index_by = array(0 => ''), $on_collision    = ON_COLLISION_ABORT, $rec_len         = 1024) {
   $handle = fopen($csv_file, 'r');
   if($handle == null || ($data = fgetcsv($handle, $rec_len, $separator_input)) === false)
   {
       // Couldn't open/read from CSV file.
       return -1;
   }

   $names = array();
   foreach($data as $field)
   {
       $names[] = trim($field);
   }

   $indexes = array();
   foreach($index_by as $index_in => $function)
   {
       if(is_int($index_in))
       {
           if($index_in < 0 || $index_in > count($data))
           {
               // Index out of bounds.
               fclose($handle);
               return -2;
           }

           $index_out = $index_in;
       }
       else
       {
           // If a column that is used as part of the key to the hash table is supplied
           // as a name rather than as an integer, then determine that named column's
           // integer index in the $names array, because the integer index is used, below.
           $get_index = array_keys($names, $index_in);
           $index_out = $get_index[0];

           if(is_null($index_out))
           {
               // A column name was given (as opposed to an integer index), but the
               // name was not found in the first row that was read from the CSV file.
               fclose($handle);
               return -3;
           }
       }

       $indexes[$index_out] = $function;
   }

   if(count($indexes) == 0)
   {
       // No columns were supplied to index by.
       fclose($handle);
       return -4;
   }

   $retval = array();
   while(($data = fgetcsv($handle, $rec_len, $separator_input)) !== false)
   {
       $index_by = '';
       foreach($indexes as $index => $function)
       {
           $index_by .= ($function ? $function($data[$index]) : $data[$index]) . $separator_index;
       }
       $index_by = substr($index_by, 0, -1);

       if(isset($retval[$index_by]))
       {
           switch($on_collision)
           {
               case ON_COLLISION_OVERWRITE     : $retval[$index_by] = array_combine($names, $data);
               case ON_COLLISION_SKIP          : break;
               case ON_COLLISION_ABORT         : return -5;
           }
       }
       else
       {
           $retval[$index_by] = array_combine($names, $data);
       }
   }
   fclose($handle);

   return $retval;
} 

/**
 * Saves a csv file to the specified path
 *
 * @param $filepath
 *   Array of strings.
 *
 * @param $headers
 *   Array of strings.
 *
 * @param $values
 *   Array of strings.
 *
 * @param $file_options
 *   Array with 'record separator', 'field separator', 'text delimiter'
 *   and 'escape character'. If not set, the options default to the
 *   CSV options ("\n", ',', '"', '"').
 *
 * @return
 *   String.
 */
function modulebrowser_export_to_csv($filepath, $filename, $headers, $data) {
  // Store somewhere, so it can be accessed generally
  $file_options = array(
    'file_format' => 'csv',
    'record separator' => '<newline>',
    'other record separator' => '<newline>',
    'field separator' => ',',
    'other field separator' => ',',
    'text delimiter' => '"',
    'other text delimiter' => '"',
    'escape character' => '"',
    'other escape character' => '"',
    'mime' => 'text/comma-separated-values',
  ); 
  
  $output = '';
  if (!empty($headers)) {
    $output = modulebrowser_write_to_string($headers, $file_options);
  }
  if (!empty($data)) {
    foreach ($data as $row) {
      $output .= modulebrowser_write_to_string($row, $file_options);
    }
  }
  if (empty($output)) {
    return 0;
  }
  return file_save_data($output, $filepath . '/' . $filename, FILE_EXISTS_REPLACE);
}

/**
 * Returns one line in the specified file format of the array of
 * values.
 *
 * @param $values
 *   Array of strings.
 *
 * @param $file_options
 *   Array with 'record separator', 'field separator', 'text delimiter'
 *   and 'escape character'. If not set, the options default to the
 *   CSV options ("\n", ',', '"', '"').
 *
 * @return
 *   String.
 */
function modulebrowser_write_to_string($values, $file_options) {
  // File options.
  _modulebrowser_sanitize_file_options($file_options);

  $rs = $file_options['record separator'];
  $fs = $file_options['field separator'];
  $td = $file_options['text delimiter'];
  $ec = $file_options['escape character'];

  // Write data.
  $output = '';

  if (is_array($values) && !empty($values)) {
    // TODO: we could avoid writing $td if the $value does not contain $td, $fs or $rs.
    if (drupal_strlen($td) > 0) {
      foreach ($values as $i => $value) {
        $values[$i] = $td . str_replace($td, $ec . $td, $value) . $td;
      }
    }
    $output = implode($fs, $values);
  }

  return $output . $rs;
}

/**
 * 
 * @param array $file_options
 *    Options to read the file with.
 * @return array
 *    Necassary file options.
 */
function _modulebrowser_sanitize_file_options(&$file_options) {
  // File options.
  $replaces = array('<newline>' => "\n", '<tab>' => "\t", '<none>' => '');
  $options = array(
    'record separator' => '<newline>',
    'field separator' => ',',
    'text delimiter' => '"',
    'escape character' => '"',
  );
  foreach ($options as $key => $default) {
    if (isset($file_options[$key]) && strlen($file_options[$key]) > 0) {
      $options[$key] = $file_options[$key];
    }
    else if (isset($file_options['other '. $key]) && strlen($file_options['other '. $key]) > 0) {
      $options[$key] = $file_options['other '. $key];
    }
  }
  $file_options = str_replace(array_keys($replaces), array_values($replaces), $options);
}
