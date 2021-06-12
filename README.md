#JPack



## Example
```cpp
JsonView archive(my_json);
// my_json = {
//   "val1": 3,
//   "myobj": {
//     "val2": 2
//   }
// }

int value1;
int value2;
int value3;

archive >> Formats(
  Must("val1", Ref(value1)),     // value1 is 3
  Must("myobj", Formats(    // access to nested
    Should("val2", Ref(value2))  // "val2" exist. not throw. value2 is 2
  )),
  Might("val3", Ref(value3))     // "val3" not exist. do nothing.
);
```

## API

### JsonView
JsonView is named requirement.

|Expression|Return|Description|
|---|---|---|
|has(AccessKey)|true if exist. otherwise false.|check if child exists with AccessKey on JsonView.|
| operator\[\](AccessKey)|||

### Formats
Forward unpack operation to each of child.

### Accessor
|Type|Description|
|---|---|
| Must(AccessKey, Target) | unpack with jsonview[AccessKey] to Target |
| Should(AccessKey, Target) | unpack with jsonview[AccessKey] to Target. if AccessKey not exist on jsonview, throw exception.|
| Might (AccessKey, Target) | unpack with jsonview[AccessKey] to Target. if AccessKey not exist on jsonview, do nothing.|


## Custom Struct

## Custom Accessor

## Grammar
```
Formats -> Accessor + Formats |  (empty)
Accessor -> AccessKey + Target
Target -> Ref |  Formats
```
