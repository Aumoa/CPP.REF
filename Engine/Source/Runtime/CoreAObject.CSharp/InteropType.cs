// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Reflection;
using System.Runtime.InteropServices;

namespace AE.CoreAObject;

/// <inheritdoc/>
public class InteropType : Type
{
    private readonly Type managedType;
    private readonly IntPtr nativePointer;

    /// <inheritdoc/>
    public InteropType(Type managedType, IntPtr nativePointer)
    {
        this.managedType = managedType;
        this.nativePointer = nativePointer;
    }

    /// <inheritdoc/>
    public override Assembly Assembly => managedType.Assembly;

    /// <inheritdoc/>
    public override string? AssemblyQualifiedName => managedType.AssemblyQualifiedName;

    /// <inheritdoc/>
    public override Type? BaseType => managedType.BaseType;

    /// <inheritdoc/>
    public override string? FullName => managedType.FullName;

    /// <inheritdoc/>
    public override Guid GUID => managedType.GUID;

    /// <inheritdoc/>
    public override Module Module => managedType.Module;

    /// <inheritdoc/>
    public override string? Namespace => managedType.Namespace;

    /// <inheritdoc/>
    public override Type UnderlyingSystemType => managedType.UnderlyingSystemType;

    /// <inheritdoc/>
    public override string Name => managedType.Name;

    /// <inheritdoc/>
    public override ConstructorInfo[] GetConstructors(BindingFlags bindingAttr)
    {
        return managedType.GetConstructors(bindingAttr);
    }

    /// <inheritdoc/>
    public override object[] GetCustomAttributes(bool inherit)
    {
        return managedType.GetCustomAttributes(inherit);
    }

    /// <inheritdoc/>
    public override object[] GetCustomAttributes(Type attributeType, bool inherit)
    {
        return managedType.GetCustomAttributes(attributeType, inherit);
    }

    /// <inheritdoc/>
    public override Type? GetElementType()
    {
        return managedType.GetElementType();
    }

    /// <inheritdoc/>
    public override EventInfo? GetEvent(string name, BindingFlags bindingAttr)
    {
        return managedType.GetEvent(name, bindingAttr);
    }

    /// <inheritdoc/>
    public override EventInfo[] GetEvents(BindingFlags bindingAttr)
    {
        return managedType.GetEvents(bindingAttr);
    }

    /// <inheritdoc/>
    public override FieldInfo? GetField(string name, BindingFlags bindingAttr)
    {
        return managedType.GetField(name, bindingAttr);
    }

    /// <inheritdoc/>
    public override FieldInfo[] GetFields(BindingFlags bindingAttr)
    {
        return managedType.GetFields(bindingAttr);
    }

    /// <inheritdoc/>
    [return: DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.Interfaces)]
    public override Type? GetInterface(string name, bool ignoreCase)
    {
        return managedType.GetInterface(name, ignoreCase);
    }

    /// <inheritdoc/>
    public override Type[] GetInterfaces()
    {
        return managedType.GetInterfaces();
    }

    /// <inheritdoc/>
    public override MemberInfo[] GetMembers(BindingFlags bindingAttr)
    {
        return managedType.GetMembers(bindingAttr);
    }

    /// <inheritdoc/>
    public override MethodInfo[] GetMethods(BindingFlags bindingAttr)
    {
        return managedType.GetMethods(bindingAttr);
    }

    /// <inheritdoc/>
    public override Type? GetNestedType(string name, BindingFlags bindingAttr)
    {
        return managedType.GetNestedType(name, bindingAttr);
    }

    /// <inheritdoc/>
    public override Type[] GetNestedTypes(BindingFlags bindingAttr)
    {
        return managedType.GetNestedTypes(bindingAttr);
    }

    /// <inheritdoc/>
    public override PropertyInfo[] GetProperties(BindingFlags bindingAttr)
    {
        return managedType.GetProperties(bindingAttr);
    }

    /// <inheritdoc/>
    public override object? InvokeMember(string name, BindingFlags invokeAttr, Binder? binder, object? target, object?[]? args, ParameterModifier[]? modifiers, CultureInfo? culture, string[]? namedParameters)
    {
        return managedType.InvokeMember(name, invokeAttr, binder, target, args, modifiers, culture, namedParameters);
    }

    /// <inheritdoc/>
    public override bool IsDefined(Type attributeType, bool inherit)
    {
        return managedType.IsDefined(attributeType, inherit);
    }

    /// <inheritdoc/>
    protected override TypeAttributes GetAttributeFlagsImpl()
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc/>
    protected override ConstructorInfo? GetConstructorImpl(BindingFlags bindingAttr, Binder? binder, CallingConventions callConvention, Type[] types, ParameterModifier[]? modifiers)
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc/>
    protected override MethodInfo? GetMethodImpl(string name, BindingFlags bindingAttr, Binder? binder, CallingConventions callConvention, Type[]? types, ParameterModifier[]? modifiers)
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc/>
    protected override PropertyInfo? GetPropertyImpl(string name, BindingFlags bindingAttr, Binder? binder, Type? returnType, Type[]? types, ParameterModifier[]? modifiers)
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc/>
    protected override bool HasElementTypeImpl()
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc/>
    protected override bool IsArrayImpl()
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc/>
    protected override bool IsByRefImpl()
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc/>
    protected override bool IsCOMObjectImpl()
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc/>
    protected override bool IsPointerImpl()
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc/>
    protected override bool IsPrimitiveImpl()
    {
        throw new NotImplementedException();
    }

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Type_Construct", CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr NativeCall_Construct(IntPtr self);
}
