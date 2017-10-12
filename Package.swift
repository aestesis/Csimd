// swift-tools-version:4.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Csmd",
    products: [
        .library(
            name: "Csmd",type: .static,targets: ["Csmd"]),
        ],
    targets: [
        .target(
            name:"Csmd",dependencies:[],path:"Sources/simdlib"
        )
    ]
)