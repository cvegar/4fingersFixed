const fs = require('fs'),
  path = require("path");

const workspaceSettings = `<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
\t<key>BuildSystemType</key>
\t<string>Original</string>
</dict>
</plist>
`;

// Returns the project name
function getProjectName(protoPath) {
  const
    cordovaConfigPath = path.join(protoPath, 'config.xml'),
    content = fs.readFileSync(cordovaConfigPath, 'utf-8');
  return /<name>([\s\S]*)<\/name>/mi.exec(content)[1].trim();
}

module.exports = function (context) {
  const
    projectRoot = context.opts.projectRoot,
    projectName = getProjectName(projectRoot),
    workspaceSettingsPath = path.join(projectRoot, `/platforms/ios/${projectName}.xcworkspace/xcshareddata/WorkspaceSettings.xcsettings`);
  fs.writeFileSync(workspaceSettingsPath, workspaceSettings);
}