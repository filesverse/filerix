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
  mpa: true,
  head: [
    [
      'link', 
      { rel: 'icon', href: 'favicon.ico' }
    ],
    ['meta', { name: 'google-site-verification', content: 'Y102h-h5HYlYeh1duBU7Tiw4hWdBh3kJcE42t1goHKI' }]
  ],
  sitemap: {
    hostname: 'https://filesverse.github.io/filerix'
  },
  themeConfig: {
    logo: '/logo.png',

    nav: [
      { text: 'Home', link: '/' },
      { text: "Guide", link: '/guide/installation' },
      { text: "References", link: '/references/compression' },
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
          { text: '<i class="devicon-electron-original colored"></i> Electron', link: '/guide/usage/electron' }
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
