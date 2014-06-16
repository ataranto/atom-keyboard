{
  'target_defaults': {
    'conditions': [
      ['OS=="win"', {
        'msvs_disabled_warnings': [

        ],
      }],
    ],
  },
  'targets': [
    {
      'target_name': 'keyboard',
      'sources': [
        'src/main.cc',
      ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")'
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [

          ],
          'libraries': [
            '-lkernel32.lib',
            '-luser32.lib'
          ],
        }],
        ['OS=="mac"', {
          'sources': [

          ],
          'libraries': [

          ],
        }],
        ['OS not in ["mac", "win"]', {
          'sources': [

          ],
        }],
      ],
    }
  ]
}
