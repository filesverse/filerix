import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "libfm",
  description: "File manager library that provides all the essentials.",
  themeConfig: {
    logo: '/logo.png',
    nav: [
      { text: 'Home', link: '/' },
      { text: "Guide", link: '/guide/installation' },
    ],

    sidebar: [
      {
        text: 'Introduction',
        items: [
          { text: 'installation', link: '/guide/installation' },
          { text: 'Getting Started', link: '/guide/getting-started' },
          { text: 'Logging', link: '/guide/logging' }
        ]
      },
      {
        text: 'Usage',
        items: [
          {
            text: 'c++',
            link: "#",
            items: [
              { text: 'cmake', link: '/guide/usage/cpp-cmake' },
              { text: 'meson', link: '/guide/usage/cpp-meson' },
            ]
          },
          {
            text: 'nodejs',
            link: '/guide/usage/nodejs',
          },
          {
            text: 'electron',
            link: '/guide/usage/electron',
          },
        ]
      },
      {
        text: 'References',
        items: [
          { text: 'Compression', link: '/guide/references/compression' },
          { text: 'Drives', link: '/guide/references/drives' },
          { text: 'Filesystem', link: '/guide/references/filesystem' },
          { text: 'User', link: '/guide/references/user' },
        ]
      },
    ],
    socialLinks: [
      { icon: 'github', link: 'https://github.com/KingMaj0r/libfm' }
    ]
  }
})
