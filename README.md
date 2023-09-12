# Hanvon EBook Hacker

[![GitHub license](https://img.shields.io/github/license/fankes/HanvonEBookHacker?color=blue)](https://github.com/fankes/HanvonEBookHacker/blob/master/LICENSE)

汉王 Windows CE 6 (MIPS) 系列电纸书越狱教程。

这是一个本应该于 2014-2015 年就完成的项目，现在作为我对当年研究成果的情怀，重新编写并开源。

得益于其系统使用的是 Windows 嵌入式平台，可编程并具有些许软件生态，对于其机带系统内容并未做加密且全部写在一张 TF 卡上，越狱难度和成本相对较低，且有部分 MIPS 的应用程序例如 CorePlayer 可拓展这台岁月历史的机器一些可玩性。

现如今 Windows CE 设备已被 Android 市场取代，微软也放弃了这个系统，这也是一个时代的终结。

特别感谢 [MortScript](http://www.sto-helit.de/index.php?module=page&entry=ms_overview) 为自动化脚本做出的巨大贡献，目前其网站应该是停止更新了。

这些教程和资源完全是出于个人爱好编写，不提供任何可靠性保障，且仅在型号 `F30III` 上测试通过，自行拆卸机器引起的损失由您自行承担。

如您发现本教程中存在侵犯您或公司 (组织) 的任何内容，请随时联系我，我会立即进行删除。

版权所有：@fankes

## 实现原理

通过偷梁换柱法替换 `ebook.exe`，实现了免修改系统注册表自定义启动项功能。

## 快速开始

- 关机，取下机器后盖，为确保安全，断开电池电源

- 取出机带 TF 卡并使用 USB 连接电脑，进入第二个分区的 `ebook` 目录

- 重命名 `ebook` 目录下的 `ebook.exe` 为 `ebook.original.exe`

- 将 `Hacker` 目录中的所有文件放置于当前目录

- 将 `MortScript` 目录中除了 `MortRunner.exe` 和 `README.md` 之外的所有文件放置于当前目录

- 准备一张 TF 卡，大小不超过 32 GB

- 将 `Launcher` 目录放置于准备好 TF 卡根目录名为 `Programs` 的新建文件夹中

- 按照步骤 5，对放置后的 `Launcher` 目录进行相同的操作

- 将机带 TF 卡插回机器，同时插入准备好的外置 TF 卡

- 上电开机

后期你将无需再次拆下机带 TF 卡，直接使用外置 TF 卡安装你需要的应用程序即可。

## 在主程序中进入 WinCE 桌面

你只需要新建一个文件夹命名为 `进入 WinCE 桌面` 即可。

然后将这个文件夹放到“电子书库”或“PC 下载”中一个你喜欢的目录或根目录中。

点击这个文件夹即可退出主程序，回到 WinCE 桌面，这一切的前提为你并未自己修改 `Launcher` 中的相关脚本并且确保已正确启动 `explorer.exe`。

这一过程大概需要 3-7 秒 (取决于你是否设置了开机画面)，在这期间会重新出现主程序界面，请耐心等待完全进入 WinCE 桌面后再使用。

你可以修改 `Launcher` 目录中的 `Daemon.mscr` 来自定义监听的名称。

### 实现原理

通过监听 `ebook.exe` 对自身窗口名称的更新来判断当前用户的操作。

反编译 `ebook.exe` 可能花费的精力会更大，且不值得。

所以这个方案就很完美的利用了这个漏洞来实现随时都可以从主程序中退出并进入 WinCE 桌面的功能，也没有对主程序的功能产生任何破坏行为。

## 注意事项 & 说明

- `explorer.exe` 可能无法保证在每个系统固件版本中都能启动成功，如果启动时出现应用程序发生异常的对话框，你需要手动运行一次 `nk.exe` 后再次尝试启动 (在 `Launcher.mscr` 中已经预置相关功能)

- Hanvon 主程序会自动启动触摸和背光驱动，这是非常重要的功能，目前是无法手动加载对应的驱动程序的，如果不启动就会导致机器无法操作，在主程序启动后，参考 [在主程序中进入 WinCE 桌面](#在主程序中进入-wince-桌面)，就能继续使用 Windows CE

- 当主程序关闭后，背光调节以及锁屏、关机功能都将会失效，你可以重新打开主程序进行操作，这些功能受限于主程序的统一化调用管理，目前无法手动实现对它们的调用

- USB 连接功能仅支持从主程序中调用，主程序未启动的时候它只能充电，也不能连接电脑使用 ActiveAsync，目前也未能发现主板留有串口通讯的槽位

- 在 Windows CE 中，`F30III` 的实体菜单按钮相当于 `Enter` 功能

- 在 Windows CE 中，贴近屏幕边缘的触控可能会无效或偏移，这是触控技术决定的，例如可能无法点击全屏右上角的关闭按钮、顶部的菜单栏或是左下角的开始菜单按钮以及任务栏和右下角的托盘区域

- 拥有 WiFi 功能的机器，请在主程序中连接 WiFi 使其创建一个网络设备，然后参考 [在主程序中进入 WinCE 桌面](#在主程序中进入-wince-桌面)，就可以使用网络功能了

- 主程序未启动的情况下，部分界面可能会刷新不及时，此时你可以使用 `Tools` 目录中的 `ScreenRefresh` 工具来手动刷新，如果依然无法刷新，请重新启动主程序然后参考 [在主程序中进入 WinCE 桌面](#在主程序中进入-wince-桌面)

## 其它内容

你应该可以在 `Launcher` 目录的脚本中看到每一个命令的注释并可以按需进行修改。

你还可以在 `Tools` 目录中找到一些实用工具。

## 许可证

- [AGPL-3.0](https://www.gnu.org/licenses/agpl-3.0.html)

```
Copyright (C) 2017-2023 Fankes Studio(qzmmcn@163.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```

版权所有 © 2017-2023 Fankes Studio(qzmmcn@163.com)