<?php
namespace pirogue;

use Error;
use Exception;

/**
 *
 * @author Bourg, Sean P. <sean.bourg@gmail.com>
 */

/**
 * Error handler for dispatcher
 *
 * @param string $message
 * @param string $file
 * @param int $line
 */
function _dispatcher_error_handler(string $message, string $file, int $line)
{
    error_handler($message, $file, $line);
}

/**
 * Send json error message to user.
 *
 * @param
 *            ErrorException | Error $exception
 * @return string
 */
function _json_error($exception): string
{
    http_response_code(500);
    error_handler_log(database_collection_open('website'), $exception->getMessage(), $exception->getFile(), $exception->getLine());
    return $exception->getMessage();
}

/**
 * JSON request not found (404).
 * string $path Requested resource.
 *
 * @return string
 */
function _json_not_found(string $path): string
{
    http_response_code(404);
    dispatcher_set_cache_control(CACHE_CONTROL_DISABLED);
    return "Unable to find requested module: '{$path}.";
}

/**
 * Routes incoming request and return json encoded results.
 *
 * @param string $method
 * @param string $route
 * @param array $request_data
 * @param array $form_data
 * @return string
 */
function _json_route(string $method, string $route, array $request_data, array $form_data): string
{
    try {
        $_parts = explode('/', $route);
        $_module = array_shift($_parts);

        $_file = "_json/{$_module}.inc";
        if (file_exists($_file)) {
            require $_file;
            $_func = sprintf('%s\%s_%s', $_module, $method, str_replace('-', '_', array_shift($_parts)));
            $_results = function_exists($_func) ? call_user_func($_func, implode('/', $_parts), $request_data, $form_data) : _json_not_found($route);
        } else {
            $_results = _json_not_found($route);
        }
    } catch (Exception $_exception) {
        $_results = _json_error($_exception);
    } catch (Error $_exception) {
        $_results = _json_error($_exception);
    }

    return json_encode($_results);
}