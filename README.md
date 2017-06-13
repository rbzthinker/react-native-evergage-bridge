
# react-native-evergage

## Getting started

`$ npm install react-native-evergage --save`

### Mostly automatic installation

`$ react-native link react-native-evergage`

### Manual installation


#### iOS

1. In XCode, in the project navigator, right click `Libraries` ➜ `Add Files to [your project's name]`
2. Go to `node_modules` ➜ `react-native-evergage` and add `RNEvergage.xcodeproj`
3. In XCode, in the project navigator, select your project. Add `libRNEvergage.a` to your project's `Build Phases` ➜ `Link Binary With Libraries`
4. Run your project (`Cmd+R`)<

#### Android

1. Open up `android/app/src/main/java/[...]/MainActivity.java`
  - Add `import com.reactlibrary.RNEvergagePackage;` to the imports at the top of the file
  - Add `new RNEvergagePackage()` to the list returned by the `getPackages()` method
2. Append the following lines to `android/settings.gradle`:
  	```
  	include ':react-native-evergage'
  	project(':react-native-evergage').projectDir = new File(rootProject.projectDir, 	'../node_modules/react-native-evergage/android')
  	```
3. Insert the following lines inside the dependencies block in `android/app/build.gradle`:
  	```
      compile project(':react-native-evergage')
  	```

#### Windows
[Read it! :D](https://github.com/ReactWindows/react-native)

1. In Visual Studio add the `RNEvergage.sln` in `node_modules/react-native-evergage/windows/RNEvergage.sln` folder to their solution, reference from their app.
2. Open up your `MainPage.cs` app
  - Add `using Com.Reactlibrary.RNEvergage;` to the usings at the top of the file
  - Add `new RNEvergagePackage()` to the `List<IReactPackage>` returned by the `Packages` method


## Usage
```javascript
import RNEvergage from 'react-native-evergage';

// TODO: What to do with the module?
RNEvergage;
```
  