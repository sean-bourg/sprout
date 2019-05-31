<?php
use function org\employment_history_records_fetch;
use function org\employment_history_records_list;
use function org\employment_history_records_lookup;
use function pirogue\database_collection_get;
use function pirogue\import;

import('org');

function _get_database()
{
    return database_collection_get('example-site');
}

$_results = null;
$matches = [];

if ('' === $GLOBALS['.pirogue.view.path']) {
    $_results = employment_history_records_list(_get_database());
} elseif ('list' === $GLOBALS['.pirogue.view.path']) {
    $_results = employment_history_records_list(_get_database());
} elseif (preg_match('/^(\d+)$/', $GLOBALS['.pirogue.view.path'], $matches)) {
    $_results = employment_history_records_fetch(_get_database(), $matches[1]);
} elseif (preg_match('/^id\/(\d+)$/', $GLOBALS['.pirogue.view.path'], $matches)) {
    $_results = employment_history_records_fetch(_get_database(), $matches[1]);
} elseif (preg_match('/^employee[-id]?\/(\d+)$/', $GLOBALS['.pirogue.view.path'], $matches)) {
    $_results = employment_history_records_lookup(_get_database(), 'employee_id=?', [
        $matches[1]
    ]);
}

if (null === $_results) {
    http_response_code(404);
    return null;
} else {
    $_data = [
        [
            'id' => 'id',
            'employee_id' => 'employee id',
            'status' => 'status',
            'date_start' => 'date start',
            'date_end' => 'date end'
        ]
    ];

    foreach ($_results as $_row) {
        array_push($_data, [
            'id' => $_row['id'],
            'employee_id' => $_row['employee_id'],
            'status' => $_row['status'],
            'date_start' => (null == $_row['date_start']) ? null : $_row['date_start']->format('Y-m-d'),
            'date_end' => (null == $_row['date_end']) ? null : $_row['date_end']->format('Y-m-d')
        ]);
    }
    return $_data;
}
