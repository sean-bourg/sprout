<?php

/**
 * Import library files.
 * @author Bourg, Sean <sean.bourg@gmail.com>
 */
namespace pirogue;

use ErrorException;

/**
 * Base folder for library import.
 *
 * @internal
 * @var string $GLOBALS['._pirogue.import.path']
 */
$GLOBALS['._pirogue.import.path'] = '';

/**
 * Library file extension.
 *
 * @internal
 * @var string $GLOBALS['._pirogue.import.file_extension']
 */
$GLOBALS['._pirogue.import.file_extension'] = '';

/**
 * Setup import library - needed before import function can be called.
 *
 * @param string $path
 *            Base path that contains library files.
 *            
 * @param string $file_extension
 *            The file extension for library files.
 */
function __import(string $path, string $file_extension = 'inc')
{
    $GLOBALS['._pirogue.import.path'] = $path;
    $GLOBALS['._pirogue.import.file_extension'] = $file_extension;
}

/**
 * Import library file.
 *
 * @param string $name
 *            Name of library being loaded without file extension.
 * @throws ErrorException Error thrown if the requested library file is not found.
 */
function import(string $name)
{
    $file_name = sprintf('%s.%s', $name, $GLOBALS['._pirogue.import.file_extension']);
    $include_file = implode(DIRECTORY_SEPARATOR, [
        $GLOBALS['._pirogue.import.path'],
        $file_name
    ]);
    if (false == file_exists($include_file)) {
        throw new ErrorException("Unable to find library: {$name}");
    }
    include_once $include_file;
}


