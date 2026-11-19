# StarDesk Dark

An unofficial dark theme for the StarDesk remote desktop client on Windows
(built and tested against **StarDesk 1.6.0**, build 1.6.0.166).

Themes: **Dark** (grey, in the style of MuMuPlayer's dark mode), **AMOLED** (pure black),
**Windows accent colour**, and **Match Windows** (follows the Windows light/dark setting).
Text colours are tuned to meet WCAG AA contrast (4.5:1).

This is a fan-made mod. It is not made, endorsed or supported by StarDesk or NetEase.

## Two downloads

Both are in the same release on the [Releases](../../releases) page.

| Download | Who it is for | What it is |
| --- | --- | --- |
| `StarDesk-Dark-Easy-Patch.zip` | Anyone who uses StarDesk | One installer. Your normal StarDesk shortcuts open StarDesk dark. |
| `StarDesk-Dark-Developer-Patch.zip` | StarDesk's developers | The same theme written as C++/Qt source, laid out as a pull request (14 git patches). |

The release also has GitHub's automatic **Source code (zip)** download, which is this whole repository.

### Easy Patch - how to use

1. Install StarDesk 1.6.0 if you have not already.
2. Download `StarDesk-Dark-Easy-Patch.zip` from the release and unzip it.
3. Double-click **Install StarDesk Dark** and choose **Yes** when Windows asks.
4. Pick Dark or AMOLED any time from **Start > StarDesk Dark Settings**.

To remove it, run **Uninstall StarDesk Dark** from the same folder.

Nothing inside StarDesk is changed on disk. `StarDeskDark.exe` starts StarDesk and swaps its colours and
artwork in memory each time. Because it matches StarDesk's files exactly, it only works with the StarDesk
version it was built for; after a StarDesk update it needs a new build.

Windows SmartScreen may warn about the installer because it is not code-signed.

## What is in this repository

| Folder | Contents |
| --- | --- |
| `easy-patch/` | The installer and uninstaller scripts and the user README that ship in the Easy Patch. |
| `launcher-src/` | Source of `StarDeskDark.exe` (`sddark.c`), the palette and patch generators (Python), and the patch data. |
| `developer-patch/` | The developer integration: `src/` (ThemeManager, palettes, themed artwork), `docs/theme/` (design notes, `integration-points.diff`) and `PULL_REQUEST.md`. |

### Building the launcher

Needs `clang`, `lld-link`, `llvm-dlltool` and Python 3 (any OS), plus a copy of StarDesk 1.6.0:

```sh
cd launcher-src
ln -s "/path/to/StarDesk" stardesk      # the folder that contains bin/StarDesk.exe
sh build.sh                             # writes StarDeskDark.exe
```

`build.sh` rebuilds from the patch data (`*.json`) in the folder. The other Python scripts are the
one-off generators that produced that data from StarDesk 1.6.0; they are kept for reference and some
expect intermediate files that are not in the repository. `harness.c` and `verify.py` check the
patched result byte-for-byte without running Windows.

### Developer patch

StarDesk's client is closed source, so the developer patch was written without it. File paths follow
the source layout visible in the 1.6.0.166 client, new files are complete, and changes to existing
files are in `developer-patch/docs/theme/integration-points.diff`, anchored by function and marked
`[verified]`, `[likely]` or `[inferred]`. Start with `developer-patch/PULL_REQUEST.md`.

## Notes

- StarDesk, its name, logo and artwork belong to their owners. The recoloured artwork here is derived
  from StarDesk's own and is included only so the theme can be applied.
- Tested on Windows 11 with StarDesk 1.6.0.
