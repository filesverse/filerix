import { defineConfig } from 'vitepress'

const createSidebarSection = (text: string, items: { text: string, link: string }[]) => ({
  text,
  items
})

export default defineConfig({
  title: "filerix",
  description: "File manager library that provides all the essentials.",
  base: "/filerix/",
  cleanUrls: true,
  lastUpdated: true,
  head: [
    ['link', { rel: 'icon', href: '/filerix/favicon.ico' }],
    ['meta', { name: 'google-site-verification', content: 'Y102h-h5HYlYeh1duBU7Tiw4hWdBh3kJcE42t1goHKI' }],
    ['meta', { name: "title", content: 'filerix' }],
    ['meta', { name: 'description', content: 'Filerix is a lightweight and efficient library for Linux file management.' }],
    ['meta', { name: 'keywords', content: 'filerix, files-lib, file-manager, cpp-support, c-support, javascript-support, typescript-support, files' }]
  ],
  sitemap: {
    hostname: 'https://filesverse.github.io/filerix/',
  },
  themeConfig: {
    logo: '/logo.png',

    nav: [
      { text: 'Home', link: '/' },
      { text: "Guide", link: '/guide/installation' },
      { text: "Reference", link: '/references/compression' },
    ],

    sidebar: {
      '/guide/': [
        createSidebarSection('Introduction', [
          { text: 'Installation', link: '/guide/installation' },
          { text: 'Getting Started', link: '/guide/getting-started' },
          { text: 'Logging', link: '/guide/logging' }
        ]),
        createSidebarSection('Usage', [
          { text: '<i class="devicon-cplusplus-plain colored"></i> C++', link: '/guide/usage/cpp' },
          { text: '<i class="devicon-c-plain colored"></i> C', link: '/guide/usage/c' },
          { text: '<i class="devicon-nodejs-plain colored"></i> Node.js', link: '/guide/usage/nodejs' },
          { text: '<i class="devicon-electron-original colored"></i> Electron', link: '/guide/usage/electron' },
          { text: '<i class="devicon-rust-plain colored" style="color:#CE422B;"></i> Rust', link: '/guide/usage/rust' },
        ])
      ],
      '/references/': [
        createSidebarSection('References', [
          { text: 'Compression', link: '/references/compression' },
          { text: 'Drives', link: '/references/drives' },
          { text: 'Filesystem', link: '/references/filesystem' },
          { text: 'User', link: '/references/user' }
        ])
      ]
    },
    socialLinks: [
      { icon: 'github', link: 'https://github.com/KingMaj0r/filerix' }
    ]
  }
})
