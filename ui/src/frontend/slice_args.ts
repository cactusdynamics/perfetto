// Copyright (C) 2023 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

import m from 'mithril';

import {Icons} from '../base/semantic_icons';
import {sqliteString} from '../base/string_utils';
import {exists} from '../base/utils';
import {Actions} from '../common/actions';
import {EngineProxy} from '../common/engine';
import {ArgNode, convertArgsToTree, Key} from '../controller/args_parser';
import {Anchor} from '../widgets/anchor';
import {MenuItem, PopupMenu2} from '../widgets/menu';
import {Section} from '../widgets/section';
import {Tree, TreeNode} from '../widgets/tree';

import {addTab} from './bottom_tab';
import {globals} from './globals';
import {Arg} from './sql/args';
import {SliceDetails} from './sql/slice';
import {SqlTableTab} from './sql_table/tab';
import {SqlTables} from './sql_table/well_known_tables';

// Renders slice arguments (key/value pairs) into a Tree widget.
export function renderArguments(
    engine: EngineProxy, slice: SliceDetails): m.Children {
  if (slice.args && slice.args.length > 0) {
    const tree = convertArgsToTree(slice.args);
    return m(
        Section,
        {title: 'Arguments'},
        m(Tree, renderArgTreeNodes(engine, tree)));
  } else {
    return undefined;
  }
}

function renderArgTreeNodes(
    engine: EngineProxy, args: ArgNode<Arg>[]): m.Children {
  return args.map((arg) => {
    const {key, value, children} = arg;
    if (children && children.length === 1) {
      // If we only have one child, collapse into self and combine keys
      const child = children[0];
      const compositeArg = {
        ...child,
        key: stringifyKey(key, child.key),
      };
      return renderArgTreeNodes(engine, [compositeArg]);
    } else {
      return m(
          TreeNode,
          {
            left: renderArgKey(stringifyKey(key), value),
            right: exists(value) && renderArgValue(value),
            summary: children && renderSummary(children),
          },
          children && renderArgTreeNodes(engine, children),
      );
    }
  });
}

function renderArgKey(key: string, value?: Arg): m.Children {
  if (value === undefined) {
    return key;
  } else {
    const {key: fullKey, displayValue} = value;
    return m(
        PopupMenu2,
        {trigger: m(Anchor, {icon: Icons.ContextMenu}, key)},
        m(MenuItem, {
          label: 'Copy full key',
          icon: 'content_copy',
          onclick: () => navigator.clipboard.writeText(fullKey),
        }),
        value && m(MenuItem, {
          label: 'Find slices with same arg value',
          icon: 'search',
          onclick: () => {
            addTab({
              kind: SqlTableTab.kind,
              config: {
                table: SqlTables.slice,
                filters: [{
                  type: 'arg_filter',
                  argSetIdColumn: 'arg_set_id',
                  argName: fullKey,
                  op: `= ${sqliteString(displayValue)}`,
                }],
              },
            });
          },
        }),
        value && m(MenuItem, {
          label: 'Visualise argument values',
          icon: 'query_stats',
          onclick: () => {
            globals.dispatch(Actions.addVisualisedArg({argName: fullKey}));
          },
        }),
    );
  }
}

function renderArgValue({value}: Arg): m.Children {
  if (isWebLink(value)) {
    return renderWebLink(value);
  } else {
    return `${value}`;
  }
}

function renderSummary(children: ArgNode<Arg>[]): m.Children {
  const summary = children.slice(0, 2).map(({key}) => key).join(', ');
  const remaining = children.length - 2;
  if (remaining > 0) {
    return `{${summary}, ... (${remaining} more items)}`;
  } else {
    return `{${summary}}`;
  }
}

function stringifyKey(...key: Key[]): string {
  return key
      .map((element, index) => {
        if (typeof element === 'number') {
          return `[${element}]`;
        } else {
          return (index === 0 ? '' : '.') + element;
        }
      })
      .join('');
}

function isWebLink(value: unknown): value is string {
  return typeof value === 'string' &&
      (value.startsWith('http://') || value.startsWith('https://'));
}

function renderWebLink(url: string): m.Children {
  return m(Anchor, {href: url, target: '_blank', icon: 'open_in_new'}, url);
}